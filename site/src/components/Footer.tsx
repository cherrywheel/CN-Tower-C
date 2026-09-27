import React from 'react';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import { ExternalLink, ArrowUp, Scale, Flame } from 'lucide-react';
import { GithubIcon } from './Icons';

interface FooterProps {
  lang: Language;
  onNavigate?: (page: PageId) => void;
}

export const Footer: React.FC<FooterProps> = ({ lang, onNavigate }) => {
  const t = T[lang];

  const scrollToTop = () => {
    window.scrollTo({ top: 0, behavior: 'smooth' });
  };

  return (
    <footer className="border-t border-border bg-card/40 py-12 text-xs font-mono">
      <div className="mx-auto max-w-6xl px-4 sm:px-6">
        <div className="flex flex-col md:flex-row md:items-center md:justify-between gap-6">
          <div className="space-y-2">
            <div className="flex items-center gap-2 text-foreground font-semibold">
              <img src="./icon-flat.png" alt="icon" className="h-5 w-5 rounded-xs object-contain" />
              <span>cn tower</span>
              <span className="text-muted-foreground font-normal">
                — {t.footer.tagline}
              </span>
            </div>
            <p className="text-muted-foreground text-[11px]">
              {lang === 'be'
                ? 'тэкставая адвэнчура на чыстым c99 без бібліятэк'
                : lang === 'ru'
                ? 'текстовая адвенчура на чистом c99 без библиотек'
                : 'a text adventure in plain c99 with zero external dependencies'}
            </p>
          </div>

          <div className="flex flex-wrap items-center gap-4 text-muted-foreground">
            {onNavigate && (
              <>
                <button
                  onClick={() => onNavigate('python')}
                  className="inline-flex items-center gap-1 hover:text-foreground transition-colors cursor-pointer"
                >
                  <Flame className="h-3.5 w-3.5 text-accent" />
                  <span>{t.nav.python}</span>
                </button>

                <button
                  onClick={() => onNavigate('legal')}
                  className="inline-flex items-center gap-1 hover:text-foreground transition-colors cursor-pointer"
                >
                  <Scale className="h-3.5 w-3.5 text-accent" />
                  <span>{t.footer.legalLink}</span>
                </button>
              </>
            )}

            <a
              href="https://github.com/cherrywheel/CN-Tower-C"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-1 hover:text-foreground transition-colors"
            >
              <GithubIcon className="h-3.5 w-3.5" />
              <span>{t.footer.repoLink}</span>
            </a>

            <a
              href="https://github.com/cherrywheel/CN-Tower"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-1 hover:text-foreground transition-colors"
            >
              <ExternalLink className="h-3.5 w-3.5" />
              <span>{t.footer.pythonLink}</span>
            </a>

            <a
              href="https://github.com/varyashine/e2k-toolchain"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-1 hover:text-foreground transition-colors"
            >
              <ExternalLink className="h-3.5 w-3.5" />
              <span>{t.footer.e2kLink}</span>
            </a>

            <button
              onClick={scrollToTop}
              className="inline-flex items-center gap-1 rounded border border-border px-2 py-1 text-muted-foreground hover:text-foreground hover:border-foreground/30 transition-colors ml-auto sm:ml-0 cursor-pointer"
              title="back to top"
            >
              <ArrowUp className="h-3 w-3" />
              <span>top</span>
            </button>
          </div>
        </div>
      </div>
    </footer>
  );
};
