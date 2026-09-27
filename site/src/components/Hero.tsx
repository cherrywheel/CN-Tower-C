import React from 'react';
import DecryptedText from './DecryptedText';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import { Download, Terminal, ShieldCheck, Box, Cpu, BookOpen } from 'lucide-react';
import { GithubIcon } from './Icons';

interface HeroProps {
  lang: Language;
  onNavigate?: (page: PageId) => void;
}

export const Hero: React.FC<HeroProps> = ({ lang, onNavigate }) => {
  const t = T[lang];

  return (
    <section className="relative overflow-hidden py-16 sm:py-24">
      {/* Background radial accent glow */}
      <div className="pointer-events-none absolute -top-40 left-1/2 -z-10 h-96 w-96 -translate-x-1/2 rounded-full bg-accent/10 blur-3xl" />

      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <div className="grid items-center gap-12 lg:grid-cols-12">
          {/* Text column */}
          <div className="lg:col-span-7">
            <div className="inline-flex items-center gap-2 rounded-full border border-border bg-card px-3 py-1 font-mono text-xs text-muted-foreground shadow-xs">
              <span className="h-1.5 w-1.5 rounded-full bg-accent animate-pulse" />
              <span>{t.hero.badge}</span>
            </div>

            <h1 className="mt-4 font-mono text-4xl font-bold tracking-tight sm:text-6xl text-foreground">
              <DecryptedText
                text={t.hero.title}
                speed={50}
                maxIterations={12}
                animateOn="view"
                revealDirection="start"
                className="text-foreground"
                encryptedClassName="text-accent"
              />
            </h1>

            <p className="mt-4 text-lg font-medium text-foreground/90 sm:text-xl leading-relaxed">
              {t.hero.tagline}
            </p>

            <p className="mt-3 text-sm text-muted-foreground sm:text-base leading-relaxed">
              {t.hero.sub}
            </p>

            {/* Feature tags */}
            <div className="mt-6 flex flex-wrap gap-2 text-xs font-mono text-muted-foreground">
              <span className="inline-flex items-center gap-1.5 rounded-md border border-border bg-muted/50 px-2.5 py-1">
                <Box className="h-3.5 w-3.5 text-accent" />
                C99
              </span>
              <span className="inline-flex items-center gap-1.5 rounded-md border border-border bg-muted/50 px-2.5 py-1">
                <ShieldCheck className="h-3.5 w-3.5 text-accent" />
                {lang === 'be' ? 'без залежнасцяў' : lang === 'ru' ? 'без зависимостей' : 'zero dependencies'}
              </span>
              <span className="inline-flex items-center gap-1.5 rounded-md border border-border bg-muted/50 px-2.5 py-1">
                <Terminal className="h-3.5 w-3.5 text-accent" />
                {lang === 'be' ? 'адзін бінарнік' : lang === 'ru' ? 'один бинарник' : 'one binary'}
              </span>
              <span className="inline-flex items-center gap-1.5 rounded-md border border-border bg-muted/50 px-2.5 py-1">
                <Cpu className="h-3.5 w-3.5 text-accent" />
                {lang === 'be' ? '40+ мэтаў' : lang === 'ru' ? '40+ платформ' : '40+ targets'}
              </span>
            </div>

            {/* CTA Buttons */}
            <div className="mt-8 flex flex-wrap items-center gap-3">
              <button
                onClick={() => onNavigate?.('download')}
                className="inline-flex h-11 items-center justify-center gap-2 rounded-lg bg-accent px-5 font-mono text-sm font-semibold text-accent-foreground shadow-md transition-all hover:opacity-90 active:scale-98 cursor-pointer"
              >
                <Download className="h-4 w-4" />
                <span>{t.hero.downloadBtn}</span>
              </button>

              <button
                onClick={() => onNavigate?.('play')}
                className="inline-flex h-11 items-center justify-center gap-2 rounded-lg border border-border bg-card px-5 font-mono text-sm font-medium text-foreground shadow-xs transition-all hover:bg-muted active:scale-98 cursor-pointer"
              >
                <BookOpen className="h-4 w-4 text-accent" />
                <span>{t.hero.playBtn}</span>
              </button>

              <a
                href="https://github.com/cherrywheel/CN-Tower-C"
                target="_blank"
                rel="noopener noreferrer"
                className="inline-flex h-11 items-center justify-center gap-2 rounded-lg border border-border bg-card px-4 font-mono text-sm font-medium text-muted-foreground hover:text-foreground shadow-xs transition-all hover:bg-muted active:scale-98"
                title="GitHub Repo"
              >
                <GithubIcon className="h-4 w-4" />
                <span className="hidden sm:inline">github</span>
              </a>
            </div>
          </div>

          {/* Cover image column */}
          <div className="lg:col-span-5 flex justify-center">
            <div className="relative group w-full max-w-sm sm:max-w-md">
              <div className="absolute -inset-1 rounded-2xl bg-gradient-to-b from-accent/20 to-transparent opacity-60 blur-lg transition duration-500 group-hover:opacity-100" />
              <div className="relative overflow-hidden rounded-xl border border-border bg-card shadow-2xl">
                <img
                  src="./cover.webp"
                  alt="cn tower text adventure"
                  className="w-full object-cover transition duration-300 group-hover:scale-102"
                  loading="eager"
                />
                <div className="p-3 border-t border-border/80 bg-background/90 text-center font-mono text-[11px] text-muted-foreground">
                  <span>toronto • edge walk • 356m</span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
};
