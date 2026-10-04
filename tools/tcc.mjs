// Compile C files with Turbo C++ 1.01 in headless DOS and copy the resulting .OBJ files
// back.   node tcc.mjs <outDir> "<options>" FILE.C|FILE.ASM [...]
// .ASM files go to TASM with the options given (for example "/ml"), .C files to TCC -c.
// The DOS is the one tools/dosbackend.mjs picks (UW2_DOS: emu2, dosbox-x, staging, jsdos).
import { runStage } from "./dosbackend.mjs";
import { cpSync, mkdtempSync, mkdirSync, rmSync, writeFileSync, readdirSync, statSync, existsSync } from "node:fs";
import { tmpdir, homedir } from "node:os";
import { join, basename } from "node:path";
const [outDir, opts, ...files] = process.argv.slice(2);
// an OMF object is a chain of records (type, 16-bit length, body) ending in MODEND; the
// emulator sometimes returns a file that is empty, zero-filled or cut short, so check the
// structure and every record's checksum
function wellFormed(b) {
  let p = 0;
  while (p + 3 <= b.length) {
    const t = b[p], n = b[p + 1] | (b[p + 2] << 8);
    if (t === 0 || n === 0 || p + 3 + n > b.length) return false;
    // Turbo C writes a checksum byte that makes each record's bytes sum to zero
    let sum = 0;
    for (let i = p; i < p + 3 + n; i++) sum = (sum + b[i]) & 0xff;
    if (sum !== 0) return false;
    p += 3 + n;
    if (t === 0x8a || t === 0x8b) return p <= b.length;
  }
  return false;
}
const here = new URL(".", import.meta.url).pathname;
const stage = mkdtempSync(join(tmpdir(), "tcc-"));
cpSync(process.env.UW2DECOMP_TC || join(here, "..", "TC"), stage, { recursive: true });
// TASM 2.0, when present, sits beside TCC: .ASM files are assembled with it, and TCC needs it
// for C files with inline asm
const tasmDir = process.env.UW2DECOMP_TASM || join(here, "..", "TASM");
try { cpSync(join(tasmDir, "TASM.EXE"), join(stage, "TASM.EXE")); } catch { }
// the shared headers in src/include sit beside the sources, where TCC finds #include "name.h"
// (the current directory, and -IC:\); a name TC itself has would replace TC's header. So does
// portable.h, from Exhume's runtime/include, found as tools/exhume.py finds it: $EXHUME, else
// .exhume in this repository, else ~/Exhume
const incDir = process.env.UW2DECOMP_INCLUDE || join(here, "..", "src", "include");
const exhume = process.env.EXHUME || (existsSync(join(here, "..", ".exhume", "runtime")) ? join(here, "..", ".exhume") : join(homedir(), "Exhume"));
const rtInc = join(exhume, "runtime", "include");
for (const [dir, label] of [[incDir, "src/include"], [rtInc, "Exhume's runtime/include"]]) {
  let incs = [];
  try { incs = readdirSync(dir).filter(n => /\.h$/i.test(n)); } catch { }
  if (dir === rtInc && !incs.length) { console.error(`tcc.mjs: no Exhume runtime at ${exhume} (set EXHUME)`); process.exit(1); }
  for (const n of incs) {
    try { statSync(join(stage, n.toUpperCase())); console.error(`tcc.mjs: ${label}/${n} has the name of a TC file or another header`); process.exit(1); } catch { }
    cpSync(join(dir, n), join(stage, n.toUpperCase()));
  }
}
for (const f of files) cpSync(f, join(stage, basename(f).toUpperCase()));
const names = files.map(f => basename(f).toUpperCase());
writeFileSync(join(stage, "BUILD.BAT"), [
  "@echo off",
  ...names.map(n => n.endsWith(".ASM") ? `TASM ${opts} ${n} >> BUILD.LOG` : `TCC -c ${opts} -IC:\\ ${n} >> BUILD.LOG`),
  "echo DONE > DONE.TXT", ""].join("\r\n"));
// the emulator occasionally hands back empty files or never finishes; retry the run
for (let attempt = 0; attempt < 3; attempt++) {
let empty = false;
// several emulators at once can slow a build well past a minute, so allow five
const run = await runStage(stage, "BUILD.BAT", 300);
try {
  if (!run.finished) empty = true;
  mkdirSync(outDir, { recursive: true });
  for (const n of [...names.map(n => n.replace(/\.(C|ASM)$/, ".OBJ")), "BUILD.LOG"]) {
    const b = await run.read(n);
    if (!b) { console.log("missing", n); continue; }
    if (n.endsWith(".OBJ") && !wellFormed(b)) empty = true;
    writeFileSync(join(outDir, n), b);
  }
} finally { await run.close(); }
if (!empty) break;
console.log("build did not finish or left a damaged object, retrying");
}
rmSync(stage, { recursive: true, force: true });
console.log("done");
