// Run batch lines headless in DOS (dos-mcp's js-dos backend) with Turbo C++ 1.01 and
// TASM 2.0 staged in C:\, then copy named outputs back. Generic counterpart of tcc.mjs.
//
//   node tools/dosrun.mjs OUTDIR [options]
//     -f PATH[=DOSNAME]   stage a file in C:\ (DOSNAME defaults to the upper-cased basename);
//                         a directory stages its whole tree under its own name (or DOSNAME)
//     -c "LINE"           a batch line to run, in order (repeatable)
//     -o NAME             a file to copy back to OUTDIR (repeatable; DOS path relative to C:\)
//     -t SECONDS          give up on a run after this long (default 300)
//     --retries N         attempts when the run does not finish or an output is damaged (default 3)
//     --no-tc             do not stage TC/ and TASM/
//
// The run's own log is RUN.LOG (each line's output is appended there unless the line
// redirects itself); it is always copied back. An output that is missing, empty, or (for
// .OBJ) not a well-formed OMF chain, or (for .EXE) shorter than its MZ header says, counts
// as damaged and the run is retried: the emulator occasionally hands back such files.
import { JsDosBackend } from "dos-mcp/dist/backend/jsdos.js";
import { cpSync, mkdtempSync, mkdirSync, rmSync, writeFileSync, statSync } from "node:fs";
import { tmpdir } from "node:os";
import { join, basename, dirname } from "node:path";

const argv = process.argv.slice(2);
const outDir = argv.shift();
if (!outDir) { console.error("usage: node dosrun.mjs OUTDIR [-f path[=NAME]] [-c line] [-o name] [-t s]"); process.exit(2); }
const files = [], lines = [], outs = [];
let timeout = 300, retries = 3, stageTools = true;
while (argv.length) {
  const a = argv.shift();
  if (a === "-f") files.push(argv.shift());
  else if (a === "-c") lines.push(argv.shift());
  else if (a === "-o") outs.push(argv.shift());
  else if (a === "-t") timeout = Number(argv.shift());
  else if (a === "--retries") retries = Number(argv.shift());
  else if (a === "--no-tc") stageTools = false;
  else { console.error("unknown argument " + a); process.exit(2); }
}

// same check as tcc.mjs: a chain of OMF records with good checksums ending in MODEND
function omfOk(b) {
  let p = 0;
  while (p + 3 <= b.length) {
    const t = b[p], n = b[p + 1] | (b[p + 2] << 8);
    if (t === 0 || n === 0 || p + 3 + n > b.length) return false;
    let sum = 0;
    for (let i = p; i < p + 3 + n; i++) sum = (sum + b[i]) & 0xff;
    if (sum !== 0) return false;
    p += 3 + n;
    if (t === 0x8a || t === 0x8b) return p <= b.length;
  }
  return false;
}
function exeOk(b) {
  if (b.length < 0x1c || b[0] !== 0x4d || b[1] !== 0x5a) return false;
  const last = b[2] | (b[3] << 8), pages = b[4] | (b[5] << 8);
  const size = last ? (pages - 1) * 512 + last : pages * 512;
  return b.length >= size;
}
function damaged(name, b) {
  if (!b || b.length === 0) return true;
  if (/\.OBJ$/i.test(name)) return !omfOk(b);
  if (/\.EXE$/i.test(name)) return !exeOk(b);
  return false;
}

const here = new URL(".", import.meta.url).pathname;
const stage = mkdtempSync(join(tmpdir(), "dosrun-"));
if (stageTools) {
  cpSync(process.env.UW2DECOMP_TC || join(here, "..", "TC"), stage, { recursive: true });
  const tasmDir = process.env.UW2DECOMP_TASM || join(here, "..", "TASM");
  try { cpSync(join(tasmDir, "TASM.EXE"), join(stage, "TASM.EXE")); } catch { }
}
for (const f of files) {
  const eq = f.lastIndexOf("=");
  const src = eq > 0 ? f.slice(0, eq) : f;
  const name = eq > 0 ? f.slice(eq + 1) : basename(src).toUpperCase();
  const dst = join(stage, name);
  mkdirSync(dirname(dst), { recursive: true });
  cpSync(src, dst, { recursive: statSync(src).isDirectory() });
}
writeFileSync(join(stage, "RUN.BAT"), [
  "@echo off",
  ...lines.map(l => /[<>|]/.test(l) || /^(cd|copy|del|echo|if|rem|set|md|type)\b/i.test(l) ? l : `${l} >> RUN.LOG`),
  "echo DONE > DONE.TXT", ""].join("\r\n"));

let ok = false;
for (let attempt = 0; attempt < retries && !ok; attempt++) {
  if (attempt) console.log("run did not finish or left a damaged output, retrying");
  const be = new JsDosBackend({ headless: true });
  let bad = false;
  try {
    await be.loadBundle({ source: stage, autoexec: ["RUN.BAT"] });
    let done = false;
    for (let i = 0; i < timeout * 2 && !done; i++) {
      await be.wait(500);
      try { await be.fsStat("C:/DONE.TXT"); done = true; } catch { }
    }
    if (!done) { bad = true; console.log("timed out"); }
    else await be.wait(1000);   // let DOS finish writing before the files are read
    mkdirSync(outDir, { recursive: true });
    for (const n of [...outs, "RUN.LOG"]) {
      let b = null;
      try { b = await be.fsRead(`C:/${n.replace(/\\/g, "/")}`); } catch { }
      if (n !== "RUN.LOG" && damaged(n, b)) { bad = true; console.log("missing or damaged", n); }
      if (b) writeFileSync(join(outDir, basename(n.replace(/\\/g, "/"))), b);
    }
  } catch (e) { bad = true; console.log("error", e.message); }
  finally { await be.shutdown(); }
  ok = !bad;
}
rmSync(stage, { recursive: true, force: true });
console.log(ok ? "done" : "failed");
process.exit(ok ? 0 : 1);
