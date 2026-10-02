// node rungame.mjs EXE OUTPREFIX step...   steps: w:ms  k:Key[,Key]  s:name
import { JsDosBackend } from "dos-mcp/dist/backend/jsdos.js";
import { cpSync, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir, homedir } from "node:os";
import { join, dirname } from "node:path";
const [exe, prefix, ...steps] = process.argv.slice(2);
const stage = mkdtempSync(join(tmpdir(), "uw2run-"));
// the game's files: UW2_DIR, else the directory of UW2_EXE, else ~/UWGOG/UW2
const game = process.env.UW2_DIR || (process.env.UW2_EXE ? dirname(process.env.UW2_EXE) : join(homedir(), "UWGOG", "UW2"));
cpSync(game, stage, { recursive: true, filter: s => !s.includes("SAVE0.pristine") });
cpSync(exe, join(stage, "UW2.EXE"));
const be = new JsDosBackend({ headless: true });
try {
  await be.loadBundle({ source: stage, autoexec: ["UW2.EXE"] });
  for (const st of steps) {
    const [op, arg] = [st.slice(0, 1), st.slice(2)];
    if (op === "w") await be.wait(Number(arg));
    else if (op === "k") { for (const k of arg.split(",")) { await be.sendKeySequence([k]); await be.wait(400); } }
    else if (op === "s") { const r = await be.screenshot("png"); writeFileSync(`${prefix}${arg}.png`, r.bytes); console.log("shot", `${prefix}${arg}.png`); }
  }
} finally { await be.shutdown(); rmSync(stage, { recursive: true, force: true }); }
