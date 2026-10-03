// node replaydos.mjs EXE OUTDIR [--replay FILE] [--cfg FILE] [--stage DIR] [--timeout S] [step ...]
//
// Runs the replay DOS build (tools/replay.py builds it) in headless DOS, always js-dos through
// dos-mcp, as tools/rungame.mjs runs the game. With --replay, FILE goes into the game's
// directory as REPLAY.IN and the build replays it; with --cfg, FILE replaces the game's
// DATA\UW.CFG (the sound cards; js-dos has a Sound Blaster 16 at 220h, IRQ 7, DMA 1, with an
// OPL3); with --stage, DIR's files and directories (a saved game, SAVE1) are copied into the
// game's directory first; otherwise the build records, and the steps
// drive the session: w:MS wait, k:KEY[,KEY...] keys (dos-mcp's names, 400 ms apart),
// t:TEXT typed text, h:KEY,MS a key held down MS milliseconds, s:NAME a screenshot
// OUTDIR/NAME.png, m:DX,DY a relative mouse move,
// c:BUTTON,MS a click held MS milliseconds, M a corner slam (the pointer to the top left).
// A recording ends with F12, which the build takes as its stop key; replaydos sends it after
// the last step.
//
// Either way it then waits (up to --timeout seconds, 600 by default) for the game to exit
// (the batch writes DONE.TXT after UW2.EXE) and copies out RECORD.OUT, STATE.OUT and
// NULLTRAP.LOG when there are any. Then the null-pointer write check (docs/PORT.md, "Null
// pointers"): it finds DGROUP by C0's copyright string, which sits at DS:8, and writes
// DS:0..3Fh and the interrupt vector table, read with dos-mcp's read_memory after the game has
// exited, to OUTDIR/memory.json, with a screenshot of the console the game exited to
// (OUTDIR/exit.png, where C0's "Null pointer assignment" would show).
import { JsDosBackend } from "dos-mcp/dist/backend/jsdos.js";
import { cpSync, mkdtempSync, mkdirSync, rmSync, writeFileSync, readFileSync } from "node:fs";
import { tmpdir, homedir } from "node:os";
import { join, dirname } from "node:path";

const args = process.argv.slice(2);
const exe = args.shift(), out = args.shift();
let replay = null, timeout = 600, cfg = null, stageDir = null;
const steps = [];
while (args.length) {
  const a = args.shift();
  if (a === "--replay") replay = args.shift();
  else if (a === "--timeout") timeout = Number(args.shift());
  else if (a === "--cfg") cfg = args.shift();
  else if (a === "--stage") stageDir = args.shift();
  else steps.push(a);
}
mkdirSync(out, { recursive: true });
const stage = mkdtempSync(join(tmpdir(), "uw2replay-"));
const game = process.env.UW2_DIR || (process.env.UW2_EXE ? dirname(process.env.UW2_EXE) : join(homedir(), "UWGOG", "UW2"));
cpSync(game, stage, { recursive: true, filter: s => !s.includes("SAVE0.pristine") });
cpSync(exe, join(stage, "UW2.EXE"));
if (replay) cpSync(replay, join(stage, "REPLAY.IN"));
if (cfg) cpSync(cfg, join(stage, "DATA", "UW.CFG"));
if (stageDir) cpSync(stageDir, stage, { recursive: true });
const be = new JsDosBackend({ headless: true });
const t0 = Date.now();
const log = (...m) => console.log(`[${((Date.now() - t0) / 1000).toFixed(1)}s]`, ...m);
// Every call into the emulator page is bounded: a page whose emulator has stopped (DOSBox
// halts on a fatal guest error and never answers again) would otherwise hang the run. And the
// whole run is bounded by --timeout plus a minute, after which the process exits with status
// 3 whatever it is waiting on.
const bounded = (p, ms, what) => Promise.race([p, new Promise((_, rej) => setTimeout(() => rej(new Error(`${what}: no answer from the emulator in ${ms} ms`)), ms))]);
const hard = setTimeout(() => { console.error(`replaydos: gave up after ${timeout + 60} s; the emulator is not answering`); process.exit(3); }, (timeout + 60) * 1000);
hard.unref();
async function tryRead(name) {
  try { return await bounded(be.fsRead(name), 20000, `reading ${name}`); } catch (e) { if (/no answer/.test(e.message)) throw e; return null; }
}
try {
  // UWRPCK (hex): the replay build's periodic dump interval in clock ticks, UWRPFB: the 3D
  // frame buffer in every dump (src/replay/REPLAY.C)
  const env = ["UWRPCK", "UWRPTRACE", "UWRPFB"].filter(k => process.env[k]).map(k => `set ${k}=${process.env[k]}`);
  await be.loadBundle({ source: stage, autoexec: [...env, "UW2.EXE", "echo done > DONE.TXT"] });
  log(replay ? `replaying ${replay}` : "recording");
  for (const st of steps) {
    const op = st.slice(0, 1), arg = st.slice(2);
    if (op === "w") await be.wait(Number(arg));
    else if (op === "k") { for (const k of arg.split(",")) { await be.sendKeySequence([k]); await be.wait(400); } }
    else if (op === "t") await be.sendKeys(arg, 60);
    else if (op === "h") {
      const [k, ms] = arg.split(",");
      await be.page.keyboard.down(k); await be.wait(Number(ms)); await be.page.keyboard.up(k); await be.wait(200);
    }
    else if (op === "s") { const r = await be.screenshot("png"); writeFileSync(join(out, `${arg}.png`), r.bytes); log("shot", arg); }
    else if (op === "m") { const [dx, dy] = arg.split(",").map(Number); await be.moveMouseRelative(dx, dy); await be.wait(200); }
    else if (op === "M") { await be.moveMouseRelative(-4000, -4000); await be.wait(200); }
    else if (op === "c") { const [b, ms] = arg.split(","); await be.clickAtCursor(b || "left", Number(ms || 200)); await be.wait(200); }
    else throw new Error(`unknown step ${st}`);
  }
  if (!replay) { await be.sendKeySequence(["F12"]); log("F12 sent"); }
  // Wait for DONE.TXT, with a screenshot every ten seconds in OUTDIR/progress.png to show
  // where the game is.
  let done = false, shots = 0;
  while (!done && Date.now() - t0 < timeout * 1000) {
    await be.wait(2000);
    // by listing, not reading: js-dos refuses a second fsReadFile of a path ("fsGetFile
    // should not be called twice for same file"), even one that failed for want of the file
    done = (await bounded(be.fsList("/"), 20000, "fs_list")).some(f => f.name.toUpperCase() === "DONE.TXT");
    if (!done && ++shots % 5 === 0) {
      const r = await bounded(be.screenshot("png"), 20000, "screenshot");
      writeFileSync(join(out, "progress.png"), r.bytes);
    }
  }
  log(done ? "the game has exited" : "timed out waiting for the game to exit");
  for (const f of ["RECORD.OUT", "STATE.OUT", "NULLTRAP.LOG", "TRACE.OUT"]) {
    const b = await tryRead(f);
    if (b) { writeFileSync(join(out, f), b); log(`copied ${f} (${b.length} bytes)`); }
  }
  // the saved games a session made, SAVE1 to SAVE4, for comparing with the port's
  for (const d of ["SAVE1", "SAVE2", "SAVE3", "SAVE4"]) {
    let names = [];
    try { names = (await bounded(be.fsList("/" + d), 20000, "fs_list")).filter(e => !e.isDir && !/^\./.test(e.name)).map(e => e.name); } catch { }
    for (const n of names) {
      const b = await tryRead(`${d}/${n}`);
      if (b) { mkdirSync(join(out, d), { recursive: true }); writeFileSync(join(out, d, n), b); log(`copied ${d}/${n} (${b.length} bytes)`); }
    }
  }
  try { const r = await bounded(be.screenshot("png"), 20000, "screenshot"); writeFileSync(join(out, "exit.png"), r.bytes); } catch { }
  const mem = { exited: done };
  try {
    const sig = Buffer.from("Turbo C++ - Copyright 1990 Borland Intl.", "latin1");
    const hits = (await bounded(be.searchMemory(sig, { maxHits: 8, end: 0xA0000 }), 30000, "search_memory")).hits;
    mem.copyright_hits = hits;
    mem.groups = [];
    for (const h of hits) {
      const ds = h - 8;
      const b = await bounded(be.readMemory(ds, 0x40), 20000, "read_memory");
      mem.groups.push({ ds_linear: ds, ds0_3f: b.toString("hex") });
    }
    mem.ivt = (await bounded(be.readMemory(0, 0x400), 20000, "read_memory")).toString("hex");
  } catch (e) { mem.error = String(e); }
  writeFileSync(join(out, "memory.json"), JSON.stringify(mem, null, 1));
  if (!done) process.exitCode = 2;
} catch (e) {
  console.error("replaydos:", e.message);
  process.exitCode = 3;
} finally {
  try { await bounded(be.shutdown(), 30000, "shutdown"); } catch (e) { console.error("replaydos:", e.message); }
  rmSync(stage, { recursive: true, force: true });
  process.exit(process.exitCode || 0);
}
