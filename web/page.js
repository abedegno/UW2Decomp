const status = document.getElementById('status');
if (!window.crossOriginIsolated && !navigator.serviceWorker) status.textContent = 'This browser cannot run the game here (no service workers: a private window?).';
// the port's script is a classic one (MODULARIZE without EXPORT_ES6) that defines window.uwNport;
// its pthread workers load the same script again, from where document.currentScript says it came
function loadScript(src) {
  return new Promise((resolve, reject) => {
    const s = document.createElement('script');
    s.src = src; s.onload = resolve; s.onerror = () => reject(new Error(`cannot load ${src}`));
    document.head.appendChild(s);
  });
}
async function startGame(name) {
  document.getElementById('menu').hidden = true; document.getElementById('game').hidden = false;
  await loadScript(`./${name}/${name}port.js`);
  const factory = window[`${name}port`];
  const Module = await factory({
    canvas: document.getElementById('canvas'),
    locateFile: f => `./${name}/${f}`,
    onGameExit: () => location.reload(),
  });
  Module.FS.mkdir('/game');           // the game's folder: empty until Task 6 puts the data there
  Module.callMain(['--data', '/game']);
}
for (const g of ['uw1', 'uw2']) document.getElementById(`play-${g}`).onclick = () => startGame(g);
window.startGame = startGame;
