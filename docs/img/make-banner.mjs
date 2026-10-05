// Renders banner.svg (beside this file) to banner.png, 1280x640, the repository's social preview,
// in headless Chrome (puppeteer, which Exhume's npm packages bring: make setup-exhume).
//   node docs/img/make-banner.mjs
import { fileURLToPath, pathToFileURL } from "node:url";
import { dirname, join } from "node:path";
import { createRequire } from "node:module";
const here = dirname(fileURLToPath(import.meta.url));
const require = createRequire(join(here, "..", "..", "exhume", "package.json"));
const puppeteer = (await import(pathToFileURL(require.resolve("puppeteer")).href)).default;
const browser = await puppeteer.launch({ headless: true });
const page = await browser.newPage();
await page.setViewport({ width: 1280, height: 640, deviceScaleFactor: 1 });
await page.goto(pathToFileURL(join(here, "banner.svg")).href, { waitUntil: "networkidle0" });
await page.screenshot({ path: join(here, "banner.png"), clip: { x: 0, y: 0, width: 1280, height: 640 } });
await browser.close();
console.log("wrote", join(here, "banner.png"));
