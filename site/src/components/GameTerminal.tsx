import React, { useEffect, useRef, useState } from 'react';
import type { Language } from '../i18n';
import { Terminal, CornerDownLeft, RotateCcw, Play } from 'lucide-react';
import { WasiGame } from '../lib/wasi-game';
import winPath from '../../../tests/win_path.txt?raw';

interface GameTerminalProps {
  lang: Language;
}

// the same inputs ci feeds every build to reach the win, they only work with seed 1
const WIN_PATH = winPath.split('\n').map((l) => l.trim()).filter(Boolean);
const WIN_SEED = '1';

const QUICK = ['help', 'look around', 'inventory', 'go north', 'go east', 'go west', 'go south', 'back'];

const TEXT = {
  en: { placeholder: 'type a command...', loading: 'loading the wasm build...', failed: 'the wasm build failed to load', exited: 'the game quit. press restart to play again', demo: 'watch the ci run', restart: 'restart', try: 'try:' },
  ru: { placeholder: 'введите команду...', loading: 'загружаю сборку wasm...', failed: 'сборка wasm не загрузилась', exited: 'игра завершилась. нажми restart, чтобы начать заново', demo: 'прогон из ci', restart: 'заново', try: 'попробуй:' },
  be: { placeholder: 'увядзіце каманду...', loading: 'загружаю зборку wasm...', failed: 'зборка wasm не загрузілася', exited: 'гульня скончылася. націсні restart, каб пачаць нанова', demo: 'прагон з ci', restart: 'нанова', try: 'паспрабуй:' },
} as const;

export const GameTerminal: React.FC<GameTerminalProps> = ({ lang }) => {
  const t = TEXT[lang as keyof typeof TEXT] ?? TEXT.en;
  const game = useRef<WasiGame | null>(null);
  const demoTimer = useRef<number | null>(null);
  const [lines, setLines] = useState<string[]>([]);
  const [screen, setScreen] = useState('');
  const [exited, setExited] = useState(false);
  const [state, setState] = useState<'loading' | 'ready' | 'failed'>('loading');
  const [input, setInput] = useState('');
  const screenRef = useRef<HTMLPreElement>(null);

  const play = (next: string[]) => {
    if (!game.current) return;
    const r = game.current.run(next);
    setLines(next);
    setScreen(r.screen);
    setExited(r.exited);
  };

  const stopDemo = () => {
    if (demoTimer.current !== null) window.clearInterval(demoTimer.current);
    demoTimer.current = null;
  };

  useEffect(() => {
    let alive = true;
    WasiGame.load(`${import.meta.env.BASE_URL}cn_tower_game.wasm`)
      .then((g) => {
        if (!alive) return;
        game.current = g;
        setState('ready');
        play([]);
      })
      .catch(() => alive && setState('failed'));
    return () => {
      alive = false;
      stopDemo();
    };
  }, []);

  useEffect(() => {
    const el = screenRef.current;
    if (el) el.scrollTop = el.scrollHeight;
  }, [screen]);

  const send = (cmd: string) => {
    if (state !== 'ready' || exited) return;
    stopDemo();
    play([...lines, cmd]);
    setInput('');
  };

  const restart = () => {
    stopDemo();
    game.current?.newSeed();
    play([]);
  };

  const demo = () => {
    if (!game.current) return;
    stopDemo();
    game.current.setSeed(WIN_SEED);
    let i = 0;
    play([]);
    demoTimer.current = window.setInterval(() => {
      i += 1;
      play(WIN_PATH.slice(0, i));
      if (i >= WIN_PATH.length) stopDemo();
    }, 700);
  };

  return (
    <div className="rounded-xl border border-border bg-card shadow-xl overflow-hidden font-mono text-xs">
      <div className="flex flex-wrap items-center justify-between gap-2 border-b border-border bg-muted/60 px-4 py-2.5">
        <span className="font-mono text-xs text-muted-foreground flex items-center gap-1.5">
          <Terminal className="h-3.5 w-3.5 text-accent" />
          <span>cn_tower_game-wasm32-wasi.wasm</span>
        </span>
        <div className="flex items-center gap-3 text-[11px] text-muted-foreground">
          <button
            onClick={demo}
            disabled={state !== 'ready'}
            className="flex items-center gap-1 hover:text-foreground transition-colors cursor-pointer disabled:opacity-40"
          >
            <Play className="h-3 w-3" />
            {t.demo}
          </button>
          <button
            onClick={restart}
            disabled={state !== 'ready'}
            className="flex items-center gap-1 hover:text-foreground transition-colors cursor-pointer disabled:opacity-40"
          >
            <RotateCcw className="h-3 w-3" />
            {t.restart}
          </button>
        </div>
      </div>

      <pre
        ref={screenRef}
        className="h-80 overflow-y-auto p-4 bg-background/95 border-b border-border/80 whitespace-pre-wrap break-words leading-relaxed text-foreground/90"
      >
        {state === 'loading' && t.loading}
        {state === 'failed' && <span className="text-rose-400">{t.failed}</span>}
        {state === 'ready' && screen}
        {state === 'ready' && exited && <span className="text-muted-foreground">{'\n'}{t.exited}</span>}
      </pre>

      <div className="flex flex-wrap items-center gap-1.5 p-2 bg-muted/30 border-b border-border/60 text-[11px]">
        <span className="text-muted-foreground mr-1">{t.try}</span>
        {QUICK.map((c) => (
          <button
            key={c}
            onClick={() => send(c)}
            disabled={state !== 'ready' || exited}
            className="rounded bg-muted px-2 py-0.5 text-foreground/80 hover:bg-accent hover:text-accent-foreground transition-colors cursor-pointer disabled:opacity-40"
          >
            {c}
          </button>
        ))}
      </div>

      <form
        onSubmit={(e) => {
          e.preventDefault();
          send(input);
        }}
        className="flex items-center px-4 py-2.5 bg-background"
      >
        <span className="text-accent mr-2 font-bold">&gt;</span>
        <input
          id="game-terminal-input"
          type="text"
          value={input}
          onChange={(e) => setInput(e.target.value)}
          placeholder={t.placeholder}
          disabled={state !== 'ready' || exited}
          autoComplete="off"
          spellCheck={false}
          className="flex-1 bg-transparent font-mono text-xs text-foreground placeholder:text-muted-foreground/50 focus:outline-hidden"
        />
        <button
          type="submit"
          aria-label="send"
          className="ml-2 text-muted-foreground hover:text-foreground p-1 transition-colors cursor-pointer"
        >
          <CornerDownLeft className="h-3.5 w-3.5" />
        </button>
      </form>
    </div>
  );
};
