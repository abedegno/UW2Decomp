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
// The game's files and the ROMs come from web/pack.sh: NAME/NAME.data, loaded by NAME/NAME.data.js,
// Emscripten's file packager's script. That script hangs its loader on a global Module's preRun;
// the loader is handed to the port's factory, and the run dependencies it counts are counted here,
// as the port exports neither addRunDependency nor FS_createPath: the port's main runs (callMain)
// only once the files are in /game.
async function startGame(name) {
  const menu = document.getElementById('menu'), game = document.getElementById('game');
  menu.hidden = true; game.hidden = false;
  try {
    // the packager's script reads the global Module as it loads (locateFile, for the .data's
    // address) and again as the data arrives, so it stays defined
    const locateFile = f => `./${name}/${f}`;
    window.Module = { locateFile };
    await loadScript(`./${name}/${name}.data.js`);
    const loaders = window.Module.preRun || [];
    await loadScript(`./${name}/${name}port.js`);
    const factory = window[`${name}port`];
    if (typeof factory !== 'function') throw new Error(`${name}port.js did not define ${name}port`);
    let pending = 0, done; const loaded = new Promise(r => { done = r; });
    const M = await factory({
      canvas: document.getElementById('canvas'),
      locateFile,
      onGameExit: () => location.reload(),
      preRun: [M => {
        M.FS_createPath = (...a) => M.FS.createPath(...a);
        M.FS_createDataFile = (...a) => M.FS.createDataFile(...a);
        M.addRunDependency = () => { pending++; };
        M.removeRunDependency = () => { if (--pending === 0) done(); };
      }, ...loaders],
    });
    // the packager's loader fails out of sight (a rejected promise nobody holds): the first such
    // failure while the files load is the game's
    const failed = new Promise((_, no) => addEventListener('unhandledrejection', ev => no(ev.reason), { once: true }));
    failed.catch(() => {});
    if (pending) await Promise.race([loaded, failed]);
    // the home (the port's $HOME/.NAMEport) kept in the browser's IndexedDB; a first visit's is
    // seeded with settings-at-start=0, so the game starts at its title, the settings screen a key away
    const home = `/home/web_user/.${name}port`;
    M.FS.mkdirTree(home); M.FS.mount(M.IDBFS, {}, home);
    await new Promise((ok, no) => M.FS.syncfs(true, e => e ? no(new Error(`cannot read the saved files (${e})`)) : ok()));
    const cfg = `${home}/${name}port.cfg`;
    if (!M.FS.analyzePath(cfg).exists) M.FS.writeFile(cfg, 'settings-at-start=0\n');
    if (!M.FS.analyzePath('/game').exists) M.FS.mkdir('/game');
    const args = ['--data', '/game'];
    if (M.FS.analyzePath('/game/roms').exists) args.push('--mt32-roms', '/game/roms');   // the port's search does not look in subfolders
    M.callMain(args);
  } catch (e) {
    game.hidden = true; menu.hidden = false;
    status.textContent = `The game could not start: ${e && e.message ? e.message : e}`;
    console.error(e);
  }
}
for (const g of ['uw1', 'uw2']) document.getElementById(`play-${g}`).onclick = () => startGame(g);
window.startGame = startGame;
