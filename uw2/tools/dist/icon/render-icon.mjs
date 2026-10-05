// Renders an SVG to square PNGs with a transparent background, in headless Chrome (puppeteer,
// which dos-mcp brings in; npm install). Used by make-icons.py, which explains the icon files.
//   node tools/dist/icon/render-icon.mjs SVG OUTDIR SIZE...    writes OUTDIR/SIZE.png for each
import fs from 'node:fs';
import path from 'node:path';
import puppeteer from 'puppeteer';

const [svg, outdir, ...sizes] = process.argv.slice(2);
if (!svg || !outdir || !sizes.length) {
  console.error('usage: render-icon.mjs SVG OUTDIR SIZE...');
  process.exit(2);
}
const data = 'data:image/svg+xml;base64,' + fs.readFileSync(svg).toString('base64');
fs.mkdirSync(outdir, { recursive: true });
const browser = await puppeteer.launch({ headless: true });
try {
  const page = await browser.newPage();
  for (const s of sizes.map(Number)) {
    await page.setViewport({ width: s, height: s, deviceScaleFactor: 1 });
    await page.setContent(`<html><body style="margin:0;background:transparent">` +
      `<img id="i" src="${data}" width="${s}" height="${s}" style="display:block"></body></html>`);
    await page.waitForFunction(() => document.getElementById('i').complete);
    await page.screenshot({ path: path.join(outdir, `${s}.png`), omitBackground: true, clip: { x: 0, y: 0, width: s, height: s } });
  }
} finally {
  await browser.close();
}
