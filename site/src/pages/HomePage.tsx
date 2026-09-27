import React from 'react';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import { Hero } from '../components/Hero';
import { GameTerminal } from '../components/GameTerminal';
import SpotlightCard from '../components/SpotlightCard';
import { detectUserPlatform } from '../lib/detector';
import {
  Download,
  Cpu,
  Layers,
  Wrench,
  BookOpen,
  Scale,
  ArrowRight,
  Flame,
} from 'lucide-react';

interface HomePageProps {
  lang: Language;
  onNavigate: (page: PageId) => void;
}

export const HomePage: React.FC<HomePageProps> = ({ lang, onNavigate }) => {
  const t = T[lang];
  const detected = detectUserPlatform();

  const NAV_CARDS: {
    id: PageId;
    icon: React.ReactNode;
    title: string;
    desc: string;
  }[] = [
    {
      id: 'download',
      icon: <Download className="h-5 w-5 text-accent" />,
      title: t.home.cards.download.title,
      desc: t.home.cards.download.desc,
    },
    {
      id: 'targets',
      icon: <Layers className="h-5 w-5 text-accent" />,
      title: t.home.cards.targets.title,
      desc: t.home.cards.targets.desc,
    },
    {
      id: 'canary',
      icon: <Cpu className="h-5 w-5 text-accent" />,
      title: t.home.cards.canary.title,
      desc: t.home.cards.canary.desc,
    },
    {
      id: 'build',
      icon: <Wrench className="h-5 w-5 text-accent" />,
      title: t.home.cards.build.title,
      desc: t.home.cards.build.desc,
    },
    {
      id: 'play',
      icon: <BookOpen className="h-5 w-5 text-accent" />,
      title: t.home.cards.play.title,
      desc: t.home.cards.play.desc,
    },
    {
      id: 'python',
      icon: <Flame className="h-5 w-5 text-accent" />,
      title: t.home.cards.python.title,
      desc: t.home.cards.python.desc,
    },
    {
      id: 'legal',
      icon: <Scale className="h-5 w-5 text-accent" />,
      title: t.home.cards.legal.title,
      desc: t.home.cards.legal.desc,
    },
  ];

  return (
    <div>
      <Hero lang={lang} onNavigate={onNavigate} />

      {/* Quick Download Banner */}
      <section className="border-t border-border py-12 bg-muted/20">
        <div className="mx-auto max-w-6xl px-4 sm:px-6">
          <div className="rounded-2xl border border-border bg-card p-6 shadow-sm flex flex-col sm:flex-row sm:items-center sm:justify-between gap-6">
            <div className="space-y-1">
              <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
                <span className="h-1.5 w-1.5 rounded-full bg-accent" />
                <span>{t.home.quickDownloadTitle}</span>
              </div>
              <div className="font-mono text-lg font-bold text-foreground capitalize">
                {detected.osName} ({detected.arch})
              </div>
              <div className="font-mono text-xs text-muted-foreground select-all break-all">
                {detected.filename}
              </div>
            </div>

            <div className="flex flex-wrap items-center gap-3">
              <a
                href={detected.downloadUrl}
                className="inline-flex h-10 items-center justify-center gap-2 rounded-lg bg-accent px-5 font-mono text-xs font-semibold text-accent-foreground shadow-xs transition-opacity hover:opacity-90"
              >
                <Download className="h-4 w-4" />
                <span>{t.download.quickDownload}</span>
              </a>

              <button
                onClick={() => onNavigate('download')}
                className="inline-flex h-10 items-center justify-center gap-1.5 rounded-lg border border-border bg-muted/60 px-4 font-mono text-xs font-medium text-foreground hover:bg-muted transition-colors cursor-pointer"
              >
                <span>all 47 builds</span>
                <ArrowRight className="h-3.5 w-3.5" />
              </button>
            </div>
          </div>
        </div>
      </section>

      {/* Interactive Terminal Simulator Preview */}
      <section className="border-t border-border py-16">
        <div className="mx-auto max-w-6xl px-4 sm:px-6">
          <div className="mb-6 flex flex-col sm:flex-row sm:items-end sm:justify-between gap-4">
            <div>
              <div className="inline-flex items-center gap-1.5 font-mono text-xs text-accent">
                <span>preview</span>
                <span className="text-muted-foreground">//</span>
                <span>terminal sandbox</span>
              </div>
              <h2 className="mt-1 font-mono text-2xl sm:text-3xl font-bold text-foreground">
                {t.home.terminalPreviewTitle}
              </h2>
              <p className="mt-1 text-xs sm:text-sm text-muted-foreground font-mono">
                {t.home.terminalHint}
              </p>
            </div>

            <button
              onClick={() => onNavigate('play')}
              className="inline-flex items-center gap-1 font-mono text-xs text-accent hover:underline self-start sm:self-auto cursor-pointer"
            >
              <span>{t.play.heading} guide</span>
              <ArrowRight className="h-3.5 w-3.5" />
            </button>
          </div>

          <GameTerminal lang={lang} />
        </div>
      </section>

      {/* Explore Section Cards Grid */}
      <section className="border-t border-border py-16 bg-muted/10">
        <div className="mx-auto max-w-6xl px-4 sm:px-6">
          <div className="mb-8">
            <h2 className="font-mono text-2xl font-bold text-foreground">
              {t.home.explorePages}
            </h2>
          </div>

          <div className="grid gap-4 sm:grid-cols-2 lg:grid-cols-3">
            {NAV_CARDS.map((card) => (
              <SpotlightCard
                key={card.id}
                className="group p-6 flex flex-col justify-between hover:border-accent/50 transition-all cursor-pointer"
              >
                <div onClick={() => onNavigate(card.id)}>
                  <div className="mb-4 inline-flex rounded-lg border border-border bg-muted p-2.5">
                    {card.icon}
                  </div>
                  <h3 className="font-mono text-base font-bold text-foreground group-hover:text-accent transition-colors">
                    {card.title}
                  </h3>
                  <p className="mt-2 text-xs sm:text-sm text-muted-foreground leading-relaxed">
                    {card.desc}
                  </p>
                </div>

                <div
                  onClick={() => onNavigate(card.id)}
                  className="mt-6 flex items-center gap-1 font-mono text-xs font-semibold text-accent group-hover:underline pt-2 border-t border-border/50"
                >
                  <span>open</span>
                  <ArrowRight className="h-3.5 w-3.5 transition-transform group-hover:translate-x-1" />
                </div>
              </SpotlightCard>
            ))}
          </div>
        </div>
      </section>
    </div>
  );
};
