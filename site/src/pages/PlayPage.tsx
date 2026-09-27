import React from 'react';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import { GameTerminal } from '../components/GameTerminal';
import SpotlightCard from '../components/SpotlightCard';
import {
  Gamepad2,
  Keyboard,
  Sparkles,
  ArrowLeft,
  Compass,
  Lightbulb,
  Terminal,
} from 'lucide-react';

interface PlayPageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const PlayPage: React.FC<PlayPageProps> = ({ lang, onNavigate }) => {
  const t = T[lang];

  return (
    <div className="py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6 space-y-14">
        {/* Navigation & Title */}
        <div>
          <button
            onClick={() => onNavigate('home')}
            className="inline-flex items-center gap-1.5 font-mono text-xs text-muted-foreground hover:text-foreground mb-6 transition-colors cursor-pointer"
          >
            <ArrowLeft className="h-3.5 w-3.5" />
            <span>back to home</span>
          </button>

          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <span>guide</span>
            <span className="text-muted-foreground">//</span>
            <span>{t.play.heading}</span>
          </div>

          <h1 className="mt-2 font-mono text-3xl sm:text-4xl font-bold text-foreground">
            {t.play.heading}
          </h1>
          <p className="mt-2 max-w-3xl text-sm sm:text-base text-muted-foreground leading-relaxed">
            {t.play.sub}
          </p>
        </div>

        {/* Goal / Story Card */}
        <SpotlightCard className="p-6 sm:p-8 border-accent/40 bg-card">
          <div className="flex items-start gap-4">
            <div className="rounded-lg bg-accent/15 p-2.5 text-accent shrink-0">
              <Compass className="h-6 w-6" />
            </div>
            <div className="space-y-2">
              <h2 className="font-mono text-lg font-bold text-foreground">
                {t.play.goalTitle}
              </h2>
              <p className="text-xs sm:text-sm text-foreground/90 leading-relaxed font-sans">
                {t.play.goalText}
              </p>
            </div>
          </div>
        </SpotlightCard>

        {/* Playable Interactive Terminal */}
        <div className="space-y-3">
          <div className="flex items-center justify-between">
            <div className="flex items-center gap-2 font-mono text-sm font-semibold text-foreground">
              <Terminal className="h-4 w-4 text-accent" />
              <span>try commands live</span>
            </div>
            <span className="font-mono text-xs text-muted-foreground">
              real wasm build
            </span>
          </div>
          <GameTerminal lang={lang} />
        </div>

        {/* Commands & Terminal Controls side by side */}
        <div className="grid gap-8 lg:grid-cols-12">
          {/* Commands table */}
          <div className="lg:col-span-7 space-y-4">
            <div className="flex items-center gap-2 font-mono text-sm font-semibold text-foreground">
              <Gamepad2 className="h-4 w-4 text-accent" />
              <span>{t.play.cmdTableTitle}</span>
            </div>

            <div className="overflow-x-auto rounded-xl border border-border bg-card">
              <table className="w-full text-left font-mono text-xs">
                <thead className="border-b border-border bg-muted/50 text-muted-foreground">
                  <tr>
                    <th className="px-4 py-2.5 font-semibold">command</th>
                    <th className="px-4 py-2.5 font-semibold">action</th>
                  </tr>
                </thead>
                <tbody className="divide-y divide-border/60">
                  {t.play.commands.map((c) => (
                    <tr key={c.cmd} className="hover:bg-muted/30 transition-colors">
                      <td className="px-4 py-2.5 font-bold text-foreground whitespace-nowrap">
                        <code className="rounded bg-muted px-1.5 py-0.5 text-accent">{c.cmd}</code>
                      </td>
                      <td className="px-4 py-2.5 text-muted-foreground font-sans">
                        {c.desc}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>

          {/* Keybindings */}
          <div className="lg:col-span-5 space-y-4">
            <div className="flex items-center gap-2 font-mono text-sm font-semibold text-foreground">
              <Keyboard className="h-4 w-4 text-accent" />
              <span>{t.play.keysTableTitle}</span>
            </div>

            <div className="overflow-x-auto rounded-xl border border-border bg-card">
              <table className="w-full text-left font-mono text-xs">
                <thead className="border-b border-border bg-muted/50 text-muted-foreground">
                  <tr>
                    <th className="px-4 py-2.5 font-semibold">key</th>
                    <th className="px-4 py-2.5 font-semibold">behavior</th>
                  </tr>
                </thead>
                <tbody className="divide-y divide-border/60">
                  {t.play.keys.map((k) => (
                    <tr key={k.key} className="hover:bg-muted/30 transition-colors">
                      <td className="px-4 py-2 font-bold text-foreground whitespace-nowrap">
                        <kbd className="rounded border border-border bg-muted px-1.5 py-0.5 text-[11px] text-foreground">
                          {k.key}
                        </kbd>
                      </td>
                      <td className="px-4 py-2 text-muted-foreground font-sans text-xs">
                        {k.action}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>

            <div className="rounded-lg border border-border/80 bg-muted/40 p-3 text-xs font-mono text-muted-foreground leading-relaxed">
              {t.play.plainModeNote}
            </div>
          </div>
        </div>

        {/* Secrets & Tips */}
        <div className="grid gap-6 sm:grid-cols-2">
          <div className="rounded-xl border border-border bg-card p-6 space-y-4">
            <div className="flex items-center gap-2 font-mono text-sm font-bold text-foreground">
              <Lightbulb className="h-4 w-4 text-accent" />
              <span>{t.play.secretsTitle}</span>
            </div>
            <ul className="space-y-2 text-xs text-muted-foreground leading-relaxed">
              {t.play.secrets.map((s, idx) => (
                <li key={idx} className="flex items-start gap-2">
                  <span className="text-accent font-mono">•</span>
                  <span>{s}</span>
                </li>
              ))}
            </ul>
          </div>

          <div className="rounded-xl border border-border bg-card p-6 space-y-4">
            <div className="flex items-center gap-2 font-mono text-xs font-semibold text-foreground uppercase tracking-wider">
              <Sparkles className="h-4 w-4 text-accent" />
              <span>{t.play.changesTitle}</span>
            </div>
            <ul className="space-y-2 font-sans text-xs text-muted-foreground leading-relaxed">
              {t.play.changes.map((change, idx) => (
                <li key={idx} className="flex items-start gap-2">
                  <span className="text-accent font-mono">•</span>
                  <span>{change}</span>
                </li>
              ))}
            </ul>
          </div>
        </div>
      </div>
    </div>
  );
};
