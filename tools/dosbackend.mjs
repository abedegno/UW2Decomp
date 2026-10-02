// The DOS the toolchain runs in. tcc.mjs and dosrun.mjs stage a directory that becomes C:\
// (Turbo C++, TASM, the sources, a batch file ending in `echo DONE > DONE.TXT`) and call
// runStage(); this module runs the batch in one of four emulators and hands back the files.
//
//   UW2_DOS=jsdos      js-dos (DOSBox in WebAssembly, in headless Chrome, through dos-mcp)
//   UW2_DOS=dosbox-x   native DOSBox-X (`brew install dosbox-x`), headless, C: mounted on
//                      a copy of the stage
//   UW2_DOS=staging    native DOSBox Staging (~/Applications/DOSBox Staging.app or
//                      `dosbox` on PATH); its macOS build has no headless video driver, so
//                      each run opens a window for a second
//   UW2_DOS=emu2       emu2 (github.com/dmsc/emu2) with tools/emu2-date.patch, built by
//                      `make setup-emu2` into tools/emu2/: each batch line runs as its own
//                      emu2 process, no COMMAND.COM
//   unset or auto      the first of DEFAULT_ORDER that is installed, else jsdos
//
// All four give the same objects (but for the source and header time stamps Turbo C records,
// which differ between two js-dos builds too) and the same EXEs (docs/BUILDING.md,
// "Choosing the DOS").
// Binaries can be named with UW2_DOSBOX_X, UW2_DOSBOX_STAGING and UW2_EMU2.
// The game itself (rungame.mjs, dos-mcp memory reads) always runs in js-dos.
import { spawn, spawnSync } from "node:child_process";
import { cpSync, existsSync, mkdtempSync, readFileSync, rmSync, writeFileSync, openSync, closeSync, statSync } from "node:fs";
import { tmpdir, homedir } from "node:os";
import { join, dirname, delimiter } from "node:path";

export const DEFAULT_ORDER = ["emu2", "dosbox-x", "jsdos"];
const here = new URL(".", import.meta.url).pathname;

function onPath(name) {
  for (const d of (process.env.PATH || "").split(delimiter)) {
    const p = join(d, name);
    try { if (statSync(p).isFile()) return p; } catch { }
  }
  return null;
}

/** The emulator binary for a backend, or null when it is not installed. */
export function binary(name) {
  const env = { "dosbox-x": "UW2_DOSBOX_X", staging: "UW2_DOSBOX_STAGING", emu2: "UW2_EMU2" }[name];
  if (env && process.env[env]) return existsSync(process.env[env]) ? process.env[env] : null;
  if (name === "dosbox-x") return onPath("dosbox-x");
  if (name === "staging") {
    for (const p of [join(homedir(), "Applications/DOSBox Staging.app/Contents/MacOS/dosbox"),
      "/Applications/DOSBox Staging.app/Contents/MacOS/dosbox"]) if (existsSync(p)) return p;
    const p = onPath("dosbox");
    if (p && /staging/i.test(spawnSync(p, ["--version"], { encoding: "utf8" }).stdout || "")) return p;
    return null;
  }
  // only the patched build: an unpatched emu2 cannot set the date TLINK records
  if (name === "emu2") { const p = join(here, "emu2", "emu2"); return existsSync(p) ? p : null; }
  return null;
}

/** The backend in use: UW2_DOS when set (and not "auto"), else the first installed. */
export function backendName() {
  const want = (process.env.UW2_DOS || "auto").toLowerCase();
  if (want !== "auto") {
    if (!["jsdos", "dosbox-x", "staging", "emu2"].includes(want)) throw new Error(`UW2_DOS=${want}: not jsdos, dosbox-x, staging, emu2 or auto`);
    if (want !== "jsdos" && !binary(want)) throw new Error(`UW2_DOS=${want}, but it is not installed (docs/BUILDING.md)`);
    return want;
  }
  return DEFAULT_ORDER.find(n => n === "jsdos" || binary(n));
}

/**
 * Run BAT (a batch file in stage) with stage as C:\, giving up after timeout seconds.
 * Returns { finished, read(name) -> Buffer | null, close() }. finished means DONE.TXT
 * exists afterwards. The stage itself is left as it was: every run works on a copy (or, in
 * js-dos, on a virtual disk), so a retry starts from the same files.
 */
export async function runStage(stage, bat, timeout = 300) {
  const name = backendName();
  if (name === "jsdos") return runJsDos(stage, bat, timeout);
  const run = mkdtempSync(join(tmpdir(), "dosrun-c-"));
  cpSync(stage, run, { recursive: true, preserveTimestamps: true });
  const close = async () => { rmSync(run, { recursive: true, force: true }); rmSync(run + ".date", { force: true }); };
  try {
    if (name === "emu2") runEmu2(run, bat, timeout);
    else await runDosBox(name, run, bat, timeout);
  } catch (e) { await close(); throw e; }
  const read = async n => { try { return readFileSync(join(run, n.replace(/\\/g, "/"))); } catch { return null; } };
  return { finished: existsSync(join(run, "DONE.TXT")), read, close };
}

async function runJsDos(stage, bat, timeout) {
  const { JsDosBackend } = await import("dos-mcp/dist/backend/jsdos.js");
  const be = new JsDosBackend({ headless: true });
  let finished = false;
  try {
    await be.loadBundle({ source: stage, autoexec: [bat] });
    for (let i = 0; i < timeout * 2 && !finished; i++) {
      await be.wait(500);
      try { await be.fsStat("C:/DONE.TXT"); finished = true; } catch { }
    }
    if (finished) await be.wait(1000);   // let DOS finish writing before the files are read
  } catch (e) { await be.shutdown(); throw e; }
  const read = async n => { try { return Buffer.from(await be.fsRead(`C:/${n.replace(/\\/g, "/")}`)); } catch { return null; } };
  return { finished, read, close: () => be.shutdown() };
}

// DOSBox-X's defaults that matter: no sound, no long file names (the tools are DOS 3 programs),
// no emulated disk speed limit, the fastest core.
const DOSBOX_X_CONF = `[sdl]
output=surface
[cpu]
core=dynamic
cycles=max
[mixer]
nosound=true
[speaker]
pcspeaker=false
[dos]
lfn=false
hard drive data rate limit=0
keyboardlayout=us
`;

function runDosBox(name, run, bat, timeout) {
  const exe = binary(name);
  const cmds = [`mount c "${run}"`, "c:", `call ${bat}`];
  let args, env = { ...process.env };
  if (name === "dosbox-x") {
    const conf = run + ".conf";
    writeFileSync(conf, DOSBOX_X_CONF);
    args = ["-defaultconf", "-conf", conf, "-silent", "-fastlaunch", "-nogui", "-nomenu", ...[...cmds, "exit"].flatMap(c => ["-c", c])];
    env.SDL_VIDEODRIVER = "dummy"; env.SDL_AUDIODRIVER = "dummy";
  } else {
    // Staging 0.82 never returns from an `exit` given with -c, and with core=dynamic it never
    // returns from the batch on Apple Silicon: --exit and the default core
    args = ["--noprimaryconf", "--nolocalconf", "--set", "cpu_cycles=max", "--set", "mixer nosound=true",
      "--set", "startup_verbosity=quiet", ...cmds.flatMap(c => ["-c", c]), "--exit"];
  }
  return new Promise((resolve, reject) => {
    const p = spawn(exe, args, { cwd: run, env, stdio: ["ignore", "ignore", "pipe"] });
    let err = "";
    p.stderr.on("data", d => { err = (err + d).slice(-2000); });
    const t = setTimeout(() => p.kill("SIGKILL"), timeout * 1000);
    p.on("error", e => { clearTimeout(t); reject(e); });
    p.on("exit", () => { clearTimeout(t); rmSync(run + ".conf", { force: true }); resolve(); });
  });
}

// emu2 runs one program per process, so the batch is read here: `@echo off`, `echo TEXT`
// with an optional redirection, and `PROGRAM ARGS` with an optional `> FILE` or `>> FILE`.
// Anything else (cd, copy, if, a nested .BAT) is refused rather than run wrongly.
function runEmu2(run, bat, timeout) {
  const exe = binary("emu2");
  const deadline = Date.now() + timeout * 1000;
  const env = { ...process.env, EMU2_DRIVE_C: run, EMU2_DEFAULT_DRIVE: "C", EMU2_CWD: "\\",
    EMU2_DATE_FILE: run + ".date" };
  const lines = readFileSync(join(run, bat), "latin1").split(/\r?\n/);
  for (let line of lines) {
    line = line.trim();
    if (!line || /^@?echo\s+off$/i.test(line) || /^@?rem\b/i.test(line)) continue;
    const m = line.match(/^(.*?)(?:\s*(>>?)\s*(\S+))?$/);
    const [cmd, redir, file] = [m[1].trim(), m[2], m[3]];
    const out = file ? openSync(join(run, file.replace(/\\/g, "/").toUpperCase()), redir === ">>" ? "a" : "w") : "ignore";
    try {
      const echo = cmd.match(/^@?echo\s+(.*)$/i);
      if (echo) { if (file) writeFileSync(out, echo[1] + "\r\n"); continue; }
      const [prog, ...rest] = cmd.split(/\s+/);
      const found = [prog, prog + ".EXE", prog + ".COM"].map(n => n.toUpperCase()).find(n => /\.(EXE|COM)$/.test(n) && existsSync(join(run, n)));
      if (!found) throw new Error(`emu2 backend: cannot run batch line '${line}'`);
      const left = deadline - Date.now();
      if (left <= 0) return;
      spawnSync(exe, [found, ...rest], { cwd: run, env, stdio: ["ignore", out, "ignore"], timeout: left });
    } finally { if (typeof out === "number") closeSync(out); }
  }
}

// `node tools/dosbackend.mjs` prints the backend in use (tools/dosbatch.py asks this way)
if (process.argv[1] && new URL(import.meta.url).pathname === process.argv[1]) {
  try { console.log(backendName()); } catch (e) { console.error(e.message); process.exit(1); }
}
