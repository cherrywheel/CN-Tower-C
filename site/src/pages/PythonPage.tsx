import React from 'react';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import SpotlightCard from '../components/SpotlightCard';
import ShinyText from '../components/ShinyText';
import {
  ArrowLeft,
  ArrowRight,
  ExternalLink,
  Flame,
  FileCode,
  Terminal,
  Sparkles,
  GitBranch,
  Quote,
} from 'lucide-react';
import { GithubIcon } from '../components/Icons';

interface PythonPageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const PythonPage: React.FC<PythonPageProps> = ({ lang, onNavigate }) => {
  const t = T[lang];

  return (
    <div className="py-16 sm:py-24">
      <div className="mx-auto max-w-4xl px-4 sm:px-6 space-y-12">
        {/* Navigation & Header */}
        <div>
          <button
            onClick={() => onNavigate('home')}
            className="inline-flex items-center gap-1.5 font-mono text-xs text-muted-foreground hover:text-foreground mb-6 transition-colors cursor-pointer"
          >
            <ArrowLeft className="h-3.5 w-3.5" />
            <span>back to home</span>
          </button>

          <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
            <Flame className="h-3.5 w-3.5" />
            <span>{t.python.badge}</span>
          </div>

          <h1 className="mt-2 font-mono text-3xl sm:text-4xl font-extrabold text-foreground tracking-tight">
            <ShinyText
              text={t.python.heading}
              speed={3}
              color="currentColor"
              shineColor="var(--color-accent, #f43f5e)"
            />
          </h1>
          <p className="mt-2 font-mono text-sm sm:text-base text-muted-foreground">
            {t.python.sub}
          </p>
        </div>

        {/* Author Statement & Ownership Banner */}
        <div className="rounded-2xl border-2 border-accent/40 bg-accent/5 p-6 sm:p-8 backdrop-blur-xs relative overflow-hidden">
          <div className="absolute top-0 right-0 p-4 opacity-10 pointer-events-none">
            <Quote className="h-36 w-36 text-accent" />
          </div>

          <div className="relative z-10 space-y-4">
            <div className="inline-flex items-center gap-2 rounded-full border border-accent/30 bg-accent/10 px-3 py-1 font-mono text-xs font-bold text-accent">
              <Sparkles className="h-3.5 w-3.5" />
              <span>{t.python.authorNoteTitle}</span>
            </div>

            <p className="font-mono text-sm sm:text-base text-foreground leading-relaxed font-medium">
              «{t.python.authorNoteText}»
            </p>

            <div className="pt-2 flex flex-wrap items-center gap-3 font-mono text-xs text-muted-foreground border-t border-border/40">
              <span className="text-accent font-semibold">@cherrywheel</span>
              <span>•</span>
              <span>sole creator & maintainer</span>
              <span>•</span>
              <a
                href="https://github.com/cherrywheel/CN-Tower"
                target="_blank"
                rel="noopener noreferrer"
                className="inline-flex items-center gap-1 text-accent hover:underline"
              >
                <span>github/cherrywheel</span>
                <ExternalLink className="h-3 w-3" />
              </a>
            </div>
          </div>
        </div>

        {/* Story & Evolution Timeline */}
        <div className="space-y-6">
          <h2 className="font-mono text-xl sm:text-2xl font-bold text-foreground flex items-center gap-2">
            <GitBranch className="h-5 w-5 text-accent" />
            <span>{t.python.timelineTitle}</span>
          </h2>

          <div className="grid gap-6 sm:grid-cols-3">
            {t.python.timeline.map((step, idx) => (
              <SpotlightCard
                key={idx}
                className="p-6 flex flex-col justify-between border-border hover:border-accent/40 transition-colors"
              >
                <div className="space-y-3">
                  <div className="flex items-center justify-between">
                    <span className="font-mono text-2xl font-black text-accent/80">
                      {step.step}
                    </span>
                    <span className="rounded bg-muted px-2 py-0.5 font-mono text-[10px] text-muted-foreground uppercase">
                      {step.tag}
                    </span>
                  </div>

                  <h3 className="font-mono text-base font-bold text-foreground">
                    {step.title}
                  </h3>

                  <p className="text-xs sm:text-sm text-muted-foreground leading-relaxed">
                    {step.desc}
                  </p>
                </div>
              </SpotlightCard>
            ))}
          </div>
        </div>

        {/* Side-by-Side Comparison Matrix */}
        <div className="space-y-6">
          <h2 className="font-mono text-xl sm:text-2xl font-bold text-foreground flex items-center gap-2">
            <FileCode className="h-5 w-5 text-accent" />
            <span>{t.python.comparisonTitle}</span>
          </h2>

          <div className="rounded-xl border border-border bg-card overflow-hidden">
            <div className="overflow-x-auto">
              <table className="w-full text-left font-mono text-xs sm:text-sm">
                <thead>
                  <tr className="border-b border-border bg-muted/50 text-muted-foreground uppercase tracking-wider text-[11px]">
                    <th className="py-3 px-4 sm:px-6">spec / aspect</th>
                    <th className="py-3 px-4 sm:px-6 text-accent">python original</th>
                    <th className="py-3 px-4 sm:px-6 text-foreground font-bold">c99 rewrite</th>
                  </tr>
                </thead>
                <tbody className="divide-y divide-border">
                  {t.python.comparison.map((row, i) => (
                    <tr key={i} className="hover:bg-muted/30 transition-colors">
                      <td className="py-3 px-4 sm:px-6 font-semibold text-muted-foreground capitalize">
                        {row.feature}
                      </td>
                      <td className="py-3 px-4 sm:px-6 text-muted-foreground">
                        {row.python}
                      </td>
                      <td className="py-3 px-4 sm:px-6 text-foreground font-medium">
                        {row.c}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        </div>

        {/* Repositories & Quick Actions */}
        <div className="rounded-2xl border border-border bg-muted/20 p-6 sm:p-8 space-y-6">
          <h3 className="font-mono text-lg font-bold text-foreground">
            {t.python.linksTitle}
          </h3>

          <div className="grid gap-4 sm:grid-cols-2">
            <a
              href="https://github.com/cherrywheel/CN-Tower"
              target="_blank"
              rel="noopener noreferrer"
              className="flex items-center justify-between p-4 rounded-xl border border-border bg-card hover:border-accent/60 transition-all group"
            >
              <div className="flex items-center gap-3">
                <div className="rounded-lg bg-muted p-2.5">
                  <GithubIcon className="h-5 w-5 text-accent" />
                </div>
                <div>
                  <div className="font-mono text-xs font-semibold text-foreground group-hover:text-accent transition-colors">
                    cherrywheel / CN-Tower
                  </div>
                  <div className="font-mono text-[11px] text-muted-foreground">
                    original python script
                  </div>
                </div>
              </div>
              <ExternalLink className="h-4 w-4 text-muted-foreground group-hover:text-accent transition-colors" />
            </a>

            <a
              href="https://github.com/cherrywheel/CN-Tower-C"
              target="_blank"
              rel="noopener noreferrer"
              className="flex items-center justify-between p-4 rounded-xl border border-border bg-card hover:border-accent/60 transition-all group"
            >
              <div className="flex items-center gap-3">
                <div className="rounded-lg bg-muted p-2.5">
                  <GithubIcon className="h-5 w-5 text-foreground" />
                </div>
                <div>
                  <div className="font-mono text-xs font-semibold text-foreground group-hover:text-accent transition-colors">
                    cherrywheel / CN-Tower-C
                  </div>
                  <div className="font-mono text-[11px] text-muted-foreground">
                    c99 rewrite & 47 targets
                  </div>
                </div>
              </div>
              <ExternalLink className="h-4 w-4 text-muted-foreground group-hover:text-accent transition-colors" />
            </a>
          </div>

          <div className="pt-2 flex flex-wrap items-center gap-4">
            <button
              onClick={() => onNavigate('play')}
              className="inline-flex items-center gap-2 rounded-lg bg-accent px-5 py-2.5 font-mono text-xs font-semibold text-accent-foreground hover:opacity-90 transition-opacity cursor-pointer"
            >
              <Terminal className="h-4 w-4" />
              <span>{t.python.playBtn}</span>
            </button>

            <button
              onClick={() => onNavigate('download')}
              className="inline-flex items-center gap-1.5 rounded-lg border border-border bg-card px-4 py-2.5 font-mono text-xs font-medium text-foreground hover:bg-muted transition-colors cursor-pointer"
            >
              <span>{t.home.cards.download.title}</span>
              <ArrowRight className="h-3.5 w-3.5" />
            </button>
          </div>
        </div>
      </div>
    </div>
  );
};
