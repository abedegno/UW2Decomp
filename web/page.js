const status = document.getElementById('status'), progress = document.getElementById('progress');
const LOAD_STALL_MS = 120000;       // a download that moves no byte for this long brings the menu back
// The game's threads need shared memory, which needs the page cross-origin isolated: here that is
// coi-serviceworker.js's work (docs/WEB.md), which reloads the page once on a first visit. A
// browser that has no service workers, or refuses this one (cookies and site data blocked for the
// site, some private windows), never gets there, and says so here rather than at a click.
const CANNOT = 'This browser cannot run the game here: the game needs the page\'s service worker, and the browser '
             + 'does not allow it. Allow cookies and site data for this site, or use a window that is not private, then reload.';
let refused = false;                // the service worker cannot be had: the page will not be isolated
function cannotRun() {
  refused = true; status.textContent = CANNOT;
  for (const b of document.querySelectorAll('#menu button')) b.disabled = true;
}
if (!window.crossOriginIsolated) {
  const sw = navigator.serviceWorker, coi = document.querySelector('script[src*="coi-serviceworker"]');
  if (!sw || !window.isSecureContext) cannotRun();
  // the worker controls the page and still no isolation: the reload has happened and did not help
  else if (sw.controller) cannotRun();
  // asked again for the worker coi-serviceworker.js registered (the same registration): a refusal
  // means its reload is never coming. While it is accepted the page waits for that reload.
  else if (coi) sw.register(coi.src).catch(cannotRun);
}
// a message the page left for itself before it reloaded (a game's end, a stalled download)
try {
  const left = sessionStorage.getItem('exhume-message');
  if (left !== null) { sessionStorage.removeItem('exhume-message'); if (!refused) status.textContent = left; }
} catch {}
function reloadWith(message) {
  try { sessionStorage.setItem('exhume-message', message); } catch {}
  setTimeout(() => location.reload(), 300);
}
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
  let onRejection = null, stallTimer;
  const restore = message => {
    clearTimeout(stallTimer);
    if (onRejection) removeEventListener('unhandledrejection', onRejection);
    game.hidden = true; menu.hidden = false; progress.textContent = '';
    for (const b of buttons) b.disabled = refused;
    status.textContent = message;
  };
  if (!window.crossOriginIsolated) {
    // a click before the first visit's reload, or in a browser that will never be isolated
    restore(refused || navigator.serviceWorker?.controller ? CANNOT
            : 'The page is still getting ready (it reloads itself once on a first visit). Try again in a moment.');
    return;
  }
  for (const b of buttons) b.disabled = true;
  status.textContent = ''; progress.textContent = 'Loading the game: 0%';
  // The load gives up only when it has stopped moving: no byte of the download and no file
  // unpacked for LOAD_STALL_MS (window.__exhumeStallMs, for tools/webcheck.mjs), however long a
  // slow connection takes in all. A stalled load reloads the page, with what happened kept for
  // the menu, rather than leave a half-started game behind.
  const stallMs = window.__exhumeStallMs || LOAD_STALL_MS;
  let stallNow; const stalled = new Promise((_, no) => { stallNow = no; });
  stalled.catch(() => {});
  const moved = () => {
    clearTimeout(stallTimer);
    stallTimer = setTimeout(() => stallNow(Object.assign(new Error(`the download stopped (nothing arrived for ${Math.round(stallMs / 1000)} s)`), { stall: true })), stallMs);
  };
  try {
    moved();
    // the packager's script reads the global Module as it loads (locateFile, for the .data's
    // address) and again as the data arrives, so it stays defined
    const locateFile = f => `./${name}/${f}`;
    let got = -1;
    window.Module = {
      locateFile,
      setStatus: t => {
        const m = /\((\d+)\/(\d+)\)/.exec(t || '');
        if (m && +m[2]) progress.textContent = `Downloading the game: ${mb(+m[1])} of ${mb(+m[2])} MB (${Math.floor(100 * m[1] / m[2])}%)`;
        if (m && +m[1] !== got) { got = +m[1]; moved(); }
      },
    };
    await Promise.race([loadScript(`./${name}/${name}.data.js`), stalled]);
    const loaders = window.Module.preRun || [];
    await Promise.race([loadScript(`./${name}/${name}port.js`), stalled]);
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
    let unlisten = () => {};            // the page's own listeners (below), removed at the game's end
    // The game's end, once: the menu with what happened, then a fresh page for the next game (the
    // message kept across the reload). started: main is running (a failure before that is
    // callMain's, below); over: the end has begun; why: a fault that stopped the game, said then.
    let started = false, over = false, why = '', fallback;
    const end = message => {
      if (over) return; over = true; clearTimeout(fallback); unlisten();
      restore(message); reloadWith(message);
    };
    // A fault while the game runs (an abort, a trap): the port's end is asked for (exhume_quit:
    // the game paused, its files written out, the home copied, then onGameExit) while the page's
    // loop still runs; if that end does not come, the menu anyway.
    const stopped = reason => {
      if (!started || over || why) return;
      why = reason; console.error(`the game stopped: ${reason}`);
      try { M._exhume_quit(); } catch {}
      fallback = setTimeout(() => end(`The game stopped: ${why}.`), 15000);
    };
    const M = await Promise.race([factory({
      canvas: document.getElementById('canvas'),
      locateFile,
      // the game has ended (the port's loop stopped): status 0 when it quit, else the reason it
      // stopped (port_fatal, port_halt: plat_game_stop) (the port has closed its recording, written
      // out its files and had the home copied first, and no copy starts after it: the runtime has
      // ended, its file system with it)
      onGameExit: (st, reason) => end(why ? `The game stopped: ${why}.`
                                      : st ? `The game stopped: ${reason || `status ${st}`}.` : 'The game has ended.'),
      // the runtime ended without the port's end (an exit outside it): the menu at once
      onExit: code => { if (started && !over) end(`The game stopped: ${why || M.stopWhy || `it exited with status ${code}`}.`); },
      // an abort, on any of the game's threads (the runtime hands a worker's to this thread)
      onAbort: what => stopped(`it failed (${String(what).replace(/\s+/g, ' ')})`),
      preRun: [M => {
        M.FS_createPath = (...a) => M.FS.createPath(...a);
        M.FS_createDataFile = (...a) => M.FS.createDataFile(...a);
        M.addRunDependency = () => { most = Math.max(most, ++pending); };
        M.removeRunDependency = () => {
          moved();
          if (most) progress.textContent = `Loading the game: ${Math.floor(100 * (most - pending + 1) / most)}%`;
          if (--pending === 0) done();
        };
      }, ...loaders],
    }), stalled, failed]);
    window.__exhumeModule = M;      // for tools/webcheck.mjs
    if (pending) await Promise.race([loaded, failed, stalled]);
    clearTimeout(stallTimer);
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
    // an uncaught fault of the game's: a trap on the page's thread (a WebAssembly RuntimeError) or
    // on a worker (the runtime rethrows the worker's ErrorEvent on this thread); nothing else of
    // the page's throws while the game runs
    const onError = ev => {
      const e = ev.error, text = String((e && e.message) || ev.message || e || '');
      if (e instanceof WebAssembly.RuntimeError || (typeof ErrorEvent == 'function' && e instanceof ErrorEvent)
          || /RuntimeError|Aborted\(|unreachable/.test(text))
        stopped(`it failed (${text.replace(/^Uncaught /, '').replace(/\s+/g, ' ')})`);
    };
    // a right click on the game is the game's (SDL gives it mouse button 3), not the browser's
    // context menu: the menu took the button's release, and the game went on holding the button
    // (the port also lets go of every button and key the game holds when the page loses the focus)
    const onMenu = ev => ev.preventDefault();
    document.addEventListener('visibilitychange', onHidden);
    addEventListener('pagehide', onLeave);
    addEventListener('error', onError);
    game.addEventListener('contextmenu', onMenu);
    unlisten = () => { document.removeEventListener('visibilitychange', onHidden); removeEventListener('pagehide', onLeave); removeEventListener('error', onError);
                       game.removeEventListener('contextmenu', onMenu); };
    if (!M.FS.analyzePath('/game').exists) M.FS.mkdir('/game');
    const args = ['--data', '/game'];
    if (M.FS.analyzePath('/game/roms').exists) args.push('--mt32-roms', '/game/roms');   // the port's search does not look in subfolders
    menu.hidden = true; game.hidden = false;
    document.getElementById('gear').onclick = () => { document.exitPointerLock(); M._web_open_settings(); document.getElementById('canvas').focus(); };
    document.getElementById('fullscreen').onclick = () => game.requestFullscreen();
    // a port that cannot start says why in a message box (SDL's alert(), from main, on this
    // thread) and main returns its failure, which callMain returns: the message goes to the
    // status line under the menu instead of a box over a black page (or a fatal error's reason,
    // M.stopWhy, when main stopped before any box)
    let said = '';
    const alert = window.alert;
    window.alert = t => { said = String(t); console.error(said); };
    let ret;
    try { ret = M.callMain(args); } finally { window.alert = alert; }
    if (ret) { unlisten(); throw new Error(said ? said.replace(/\n+/g, ' ') : M.stopWhy || `the port stopped at its start (status ${ret})`); }
    started = true;
    // the times the sound ran dry (audio_underruns), in the console every 10 s while it changes
    let last = 0;
    const watch = setInterval(() => {
      let n;
      if (over) { clearInterval(watch); return; }
      try { n = M._audio_underruns(); } catch { clearInterval(watch); return; }     // the runtime has ended
      if (n !== last) console.log(`audio: ${n} underruns`);
      last = n;
    }, 10000);
  } catch (e) {
    const message = `The game could not start: ${e && e.message ? e.message : e}`;
    restore(message);
    console.error(e);
    // a stalled download goes on, into a half-started game: a fresh page instead
    if (e && e.stall) { for (const b of buttons) b.disabled = true; reloadWith(message); }
  }
}
for (const g of ['uw1', 'uw2']) document.getElementById(`play-${g}`).onclick = () => startGame(g);
window.startGame = startGame;
