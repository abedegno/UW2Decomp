// Compile C files with Turbo C++ 1.01 in DOS (dos-mcp js-dos, headless) and copy the
// resulting .OBJ files back.   node tcc.mjs <outDir> "<tcc options>" FILE.C [FILE2.C ...]
import { JsDosBackend } from "dos-mcp/dist/backend/jsdos.js";
import { cpSync, mkdtempSync, mkdirSync, rmSync, writeFileSync, readdirSync } from "node:fs";
import { tmpdir } from "node:os";
import { join, basename } from "node:path";
const [outDir, opts, ...files] = process.argv.slice(2);
const here = new URL(".", import.meta.url).pathname;
const stage = mkdtempSync(join(tmpdir(), "tcc-"));
cpSync(process.env.UW2DECOMP_TC || join(here, "..", "TC"), stage, { recursive: true });
for (const f of files) cpSync(f, join(stage, basename(f).toUpperCase()));
const names = files.map(f => basename(f).toUpperCase());
writeFileSync(join(stage, "BUILD.BAT"), [
  "@echo off",
  ...names.map(n => `TCC -c ${opts} -IC:\\ ${n} >> BUILD.LOG`),
  "echo DONE > DONE.TXT", ""].join("\r\n"));
// the emulator occasionally hands back empty files or never finishes; retry the run
for (let attempt = 0; attempt < 3; attempt++) {
const be = new JsDosBackend({ headless: true });
let empty = false;
try {
  await be.loadBundle({ source: stage, autoexec: ["BUILD.BAT"] });
  // several emulators at once can slow a build well past a minute, so allow five
  let done = false;
  for (let i = 0; i < 600 && !done; i++) {
    await be.wait(500);
    try { await be.fsStat("C:/DONE.TXT"); done = true; } catch { }
  }
  if (!done) empty = true;
  mkdirSync(outDir, { recursive: true });
  for (const n of [...names.map(n => n.replace(/\.C$/, ".OBJ")), "BUILD.LOG"]) {
    try { const b = await be.fsRead(`C:/${n}`); if (!b.length && n.endsWith(".OBJ")) empty = true; writeFileSync(join(outDir, n), b); } catch (e) { console.log("missing", n); }
  }
} finally { await be.shutdown(); }
if (!empty) break;
console.log("build did not finish or left an empty object, retrying");
}
rmSync(stage, { recursive: true, force: true });
console.log("done");
