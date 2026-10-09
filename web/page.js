const status = document.getElementById('status'), progress = document.getElementById('progress');
if (!window.crossOriginIsolated && !navigator.serviceWorker) status.textContent = 'This browser cannot run the game here (no service workers: a private window?).';
const LOAD_TIMEOUT_MS = 120000;     // a download that stalls this long brings the menu back
// the port's script is a classic one (MODULARIZE without EXPORT_ES6) that defines window.uwNport;
// its pthread workers load the same script again, from where document.currentScript says it came
function loadScript(src) {
  return new Promise((resolve, reject) => {
    const s = document.createElement('script');
    s.src = src; s.onload = resolve; s.onerror = () => reject(new Error(`cannot load ${src}`));
    document.head.appendChild(s);
  });
}
const mb = n => (n / 1048576).toFixed(1);
// The game's files and the ROMs come from web/pack.sh: NAME/NAME.data, loaded by NAME/NAME.data.js,
// Emscripten's file packager's script. That script hangs its loader on a global Module's preRun,
// and reports the download through that Module's setStatus ("Downloading data... (x/y)", in
// bytes; nothing when the files come from its cache in IndexedDB); the loader is handed to the
// port's factory, and the run dependencies it counts (one a file) are counted here, as the port
// exports neither addRunDependency nor FS_createPath: the port's main runs (callMain) only once the
// files are in /game. The menu, with the progress under its buttons, stays until then.
async function startGame(name) {
  const menu = document.getElementById('menu'), game = document.getElementById('game');
  const buttons = [...menu.querySelectorAll('button')];
  let onRejection = null;
  const restore = message => {
    if (onRejection) removeEventListener('unhandledrejection', onRejection);
    game.hidden = true; menu.hidden = false; progress.textContent = '';
    for (const b of buttons) b.disabled = false;
    status.textContent = message;
  };
  for (const b of buttons) b.disabled = true;
  status.textContent = ''; progress.textContent = 'Loading the game: 0%';
  try {
    // the packager's script reads the global Module as it loads (locateFile, for the .data's
    // address) and again as the data arrives, so it stays defined
    const locateFile = f => `./${name}/${f}`;
    window.Module = {
      locateFile,
      setStatus: t => {
        const m = /\((\d+)\/(\d+)\)/.exec(t || '');
        if (m && +m[2]) progress.textContent = `Downloading the game: ${mb(+m[1])} of ${mb(+m[2])} MB (${Math.floor(100 * m[1] / m[2])}%)`;
      },
    };
    await loadScript(`./${name}/${name}.data.js`);
    const loaders = window.Module.preRun || [];
    await loadScript(`./${name}/${name}port.js`);
    const factory = window[`${name}port`];
    if (typeof factory !== 'function') throw new Error(`${name}port.js did not define ${name}port`);
    // the packager's loader fails out of sight (a rejected promise nobody holds): the first such
    // failure while the files load is the game's. Listened for before the factory runs the loader,
    // and no longer once the files are in.
    let failNow; const failed = new Promise((_, no) => { failNow = no; });
    failed.catch(() => {});
    onRejection = ev => failNow(ev.reason);
    addEventListener('unhandledrejection', onRejection);
    let pending = 0, most = 0, done; const loaded = new Promise(r => { done = r; });
    let unlisten = () => {};            // the page's own copy requests (below), removed at the game's end
    const M = await factory({
      canvas: document.getElementById('canvas'),
      locateFile,
      // the game has ended (the port's loop stopped): the menu, then a fresh page for the next game
      // (the port has closed its recording, written out its files and had the home copied first)
      // (and no copy starts after it: the runtime has ended, its file system with it)
      onGameExit: () => { unlisten(); restore('The game has ended.'); setTimeout(() => location.reload(), 300); },
      preRun: [M => {
        M.FS_createPath = (...a) => M.FS.createPath(...a);
        M.FS_createDataFile = (...a) => M.FS.createDataFile(...a);
        M.addRunDependency = () => { most = Math.max(most, ++pending); };
        M.removeRunDependency = () => {
          if (most) progress.textContent = `Loading the game: ${Math.floor(100 * (most - pending + 1) / most)}%`;
          if (--pending === 0) done();
        };
      }, ...loaders],
    });
    window.__exhumeModule = M;      // for tools/webcheck.mjs
    if (pending) {
      let timer;
      const stalled = new Promise((_, no) => { timer = setTimeout(() => no(new Error(`the game's files did not arrive in ${LOAD_TIMEOUT_MS / 1000} s`)), LOAD_TIMEOUT_MS); });
      stalled.catch(() => {});
      try { await Promise.race([loaded, failed, stalled]); } finally { clearTimeout(timer); }
    }
    removeEventListener('unhandledrejection', onRejection); onRejection = null;
    // the home (the port's $HOME/.NAMEport) kept in the browser's IndexedDB; a first visit's is
    // seeded with settings-at-start=0, so the game starts at its title, the settings screen a key away
    const home = `/home/web_user/.${name}port`;
    M.FS.mkdirTree(home); M.FS.mount(M.IDBFS, {}, home);
    await new Promise((ok, no) => M.FS.syncfs(true, e => e ? no(new Error(`cannot read the saved files (${e})`)) : ok()));
    const cfg = `${home}/${name}port.cfg`;
    if (!M.FS.analyzePath(cfg).exists) M.FS.writeFile(cfg, 'settings-at-start=0\n');
    // and written back (FS.syncfs(false)): when the port asks, after it writes a setting
    // (port_config_set) and once a second while the game is changing its files (a save), when the
    // page is hidden or left, and at the game's end (the port waits for that one before it stops
    // and its runtime closes the storage). One copy at a time: a request made while one is running
    // waits for the next, which starts when that one ends. The promise says when a copy begun
    // after the request is done (true, or false if it failed).
    let syncHome;
    {
      let running = false, waiting = [];
      const run = () => {
        const these = waiting; waiting = []; running = true;
        M.FS.syncfs(false, e => {
          if (e) console.error(`the saved files could not be kept (${e})`);
          running = false;
          for (const f of these) f(!e);
          if (waiting.length) run();
        });
      };
      syncHome = () => new Promise(ok => { waiting.push(ok); if (!running) run(); });
    }
    M.syncHome = done => { syncHome().then(() => done && done()); };     // the port's request; done when copied
    const onHidden = () => { if (document.visibilityState === 'hidden') syncHome(); }, onLeave = () => syncHome();
    document.addEventListener('visibilitychange', onHidden);
    addEventListener('pagehide', onLeave);
    unlisten = () => { document.removeEventListener('visibilitychange', onHidden); removeEventListener('pagehide', onLeave); };
    if (!M.FS.analyzePath('/game').exists) M.FS.mkdir('/game');
    const args = ['--data', '/game'];
    if (M.FS.analyzePath('/game/roms').exists) args.push('--mt32-roms', '/game/roms');   // the port's search does not look in subfolders
    menu.hidden = true; game.hidden = false;
    document.getElementById('gear').onclick = () => { document.exitPointerLock(); M._web_open_settings(); document.getElementById('canvas').focus(); };
    document.getElementById('fullscreen').onclick = () => game.requestFullscreen();
    // a port that cannot start says why in a message box (SDL's alert(), from main, on this
    // thread) and main returns its failure, which callMain returns: the message goes to the
    // status line under the menu instead of a box over a black page
    let said = '';
    const alert = window.alert;
    window.alert = t => { said = String(t); console.error(said); };
    let ret;
    try { ret = M.callMain(args); } finally { window.alert = alert; }
    if (ret) throw new Error(said ? said.replace(/\n+/g, ' ') : `the port stopped at its start (status ${ret})`);
  } catch (e) {
    restore(`The game could not start: ${e && e.message ? e.message : e}`);
    console.error(e);
  }
}
for (const g of ['uw1', 'uw2']) document.getElementById(`play-${g}`).onclick = () => startGame(g);
window.startGame = startGame;
