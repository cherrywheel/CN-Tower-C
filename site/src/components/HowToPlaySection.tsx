import React from 'react';
import type { Language } from '../i18n';
import { T } from '../i18n';
import { Gamepad2, Keyboard, Sparkles } from 'lucide-react';

interface HowToPlayProps {
  lang: Language;
}

export const HowToPlaySection: React.FC<HowToPlayProps> = ({ lang }) => {
  const t = T[lang];

  return (
    <section id="play" className="border-t border-border py-16 sm:py-24">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <div>
          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <span>05</span>
            <span className="text-muted-foreground">//</span>
            <span>{t.play.heading}</span>
          </div>
          <h2 className="mt-2 font-mono text-3xl font-bold tracking-tight sm:text-4xl text-foreground">
            {t.play.heading}
          </h2>
          <p className="mt-2 max-w-2xl text-sm sm:text-base text-muted-foreground leading-relaxed">
            {t.play.sub}
          </p>
        </div>

        <div className="mt-8 grid gap-8 lg:grid-cols-12">
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
                    <th className="px-4 py-2.5 font-semibold">description</th>
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

            <div className="rounded-lg border border-border bg-muted/30 p-3 text-xs font-mono text-muted-foreground">
              <span className="text-foreground font-semibold">note: </span>
              <span>
                {lang === 'be'
                  ? 'адзін добры фінал і куча дрэнных.'
                  : lang === 'ru'
                  ? 'одна хорошая концовка и куча плохих.'
                  : 'one good ending and a bunch of bad ones.'}
              </span>
            </div>
          </div>

          {/* Terminal interactive controls & Plain mode */}
          <div className="lg:col-span-5 space-y-6">
            <div className="space-y-4">
              <div className="flex items-center gap-2 font-mono text-sm font-semibold text-foreground">
                <Keyboard className="h-4 w-4 text-accent" />
                <span>{t.play.keysTableTitle}</span>
              </div>

              <div className="overflow-x-auto rounded-xl border border-border bg-card">
                <table className="w-full text-left font-mono text-xs">
                  <thead className="border-b border-border bg-muted/50 text-muted-foreground">
                    <tr>
                      <th className="px-4 py-2.5 font-semibold">key</th>
                      <th className="px-4 py-2.5 font-semibold">action</th>
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

            {/* Changes from Python */}
            <div className="rounded-xl border border-border bg-card p-5 space-y-3">
              <div className="flex items-center gap-2 font-mono text-xs font-semibold text-foreground uppercase tracking-wider">
                <Sparkles className="h-4 w-4 text-accent" />
                <span>{t.play.changesTitle}</span>
              </div>

              <ul className="space-y-1.5 font-sans text-xs text-muted-foreground leading-relaxed">
                {t.play.changes.map((change, idx) => (
                  <li key={idx} className="flex items-start gap-1.5">
                    <span className="text-accent font-mono">•</span>
                    <span>{change}</span>
                  </li>
                ))}
              </ul>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
};
