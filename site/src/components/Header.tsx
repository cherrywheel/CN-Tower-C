import React, { useState } from 'react';
import ShinyText from './ShinyText';
import type { Language, PageId } from '../i18n';
import { T } from '../i18n';
import { Moon, Sun, Monitor, Download, Menu, X } from 'lucide-react';
import { GithubIcon } from './Icons';

interface HeaderProps {
  lang: Language;
  onSelectLang: (lang: Language) => void;
  theme: 'dark' | 'light' | 'system';
  onChangeTheme: (theme: 'dark' | 'light' | 'system') => void;
  activePage: PageId;
  onNavigate: (page: PageId) => void;
}

export const Header: React.FC<HeaderProps> = ({
  lang,
  onSelectLang,
  theme,
  onChangeTheme,
  activePage,
  onNavigate,
}) => {
  const t = T[lang];
  const [mobileMenuOpen, setMobileMenuOpen] = useState(false);

  const cycleTheme = () => {
    if (theme === 'dark') onChangeTheme('light');
    else if (theme === 'light') onChangeTheme('system');
    else onChangeTheme('dark');
  };

  const navLinks: { id: PageId; label: string }[] = [
    { id: 'home', label: t.nav.home },
    { id: 'download', label: t.nav.download },
    { id: 'targets', label: t.nav.targets },
    { id: 'canary', label: t.nav.canary },
    { id: 'build', label: t.nav.build },
    { id: 'play', label: t.nav.play },
    { id: 'python', label: t.nav.python },
    { id: 'legal', label: t.nav.legal },
  ];

  const handleNavClick = (id: PageId) => {
    onNavigate(id);
    setMobileMenuOpen(false);
  };

  return (
    <header className="sticky top-0 z-50 w-full border-b border-border/80 bg-background/90 backdrop-blur-md">
      <div className="mx-auto flex h-14 max-w-6xl items-center justify-between px-4 sm:px-6">
        <button
          onClick={() => handleNavClick('home')}
          className="flex items-center gap-2.5 font-mono text-sm font-semibold tracking-tight transition-opacity hover:opacity-80 cursor-pointer"
        >
          <img src="./icon-flat.png" alt="icon" className="h-6 w-6 rounded-sm object-contain" />
          <span className="text-foreground">
            <ShinyText text="cn tower" speed={3} color="currentColor" shineColor="var(--color-accent, #f43f5e)" />
          </span>
          <span className="hidden rounded bg-muted px-1.5 py-0.5 text-[11px] font-normal text-muted-foreground sm:inline-block">
            C99
          </span>
        </button>

        {/* Desktop Navigation */}
        <nav className="hidden items-center gap-5 text-xs font-mono lg:flex">
          {navLinks.map((link) => (
            <button
              key={link.id}
              onClick={() => handleNavClick(link.id)}
              className={`transition-colors cursor-pointer capitalize ${
                activePage === link.id
                  ? 'text-accent font-semibold'
                  : 'text-muted-foreground hover:text-foreground'
              }`}
            >
              {link.label}
            </button>
          ))}
        </nav>

        <div className="flex items-center gap-2">
          {/* Segmented language selector */}
          <div className="flex items-center rounded-md border border-border bg-muted/40 p-0.5 font-mono text-[11px]">
            {(['en', 'ru', 'be'] as const).map((l) => (
              <button
                key={l}
                onClick={() => onSelectLang(l)}
                className={`rounded px-1.5 py-0.5 uppercase transition-colors cursor-pointer ${
                  lang === l
                    ? 'bg-card text-foreground font-bold shadow-xs'
                    : 'text-muted-foreground hover:text-foreground'
                }`}
              >
                {l}
              </button>
            ))}
          </div>

          {/* Theme cycle */}
          <button
            onClick={cycleTheme}
            className="inline-flex h-8 w-8 items-center justify-center rounded-md border border-border text-muted-foreground transition-colors hover:border-foreground/30 hover:text-foreground cursor-pointer"
            title={`theme: ${theme}`}
          >
            {theme === 'dark' && <Moon className="h-3.5 w-3.5" />}
            {theme === 'light' && <Sun className="h-3.5 w-3.5" />}
            {theme === 'system' && <Monitor className="h-3.5 w-3.5" />}
          </button>

          {/* Download anchor */}
          <button
            onClick={() => handleNavClick('download')}
            className="hidden sm:inline-flex h-8 items-center gap-1.5 rounded-md bg-accent px-3 text-xs font-mono font-medium text-accent-foreground shadow-xs transition-opacity hover:opacity-90 cursor-pointer"
          >
            <Download className="h-3.5 w-3.5" />
            <span>{t.nav.download}</span>
          </button>

          {/* GitHub link */}
          <a
            href="https://github.com/cherrywheel/CN-Tower-C"
            target="_blank"
            rel="noopener noreferrer"
            className="inline-flex h-8 w-8 items-center justify-center rounded-md border border-border text-muted-foreground transition-colors hover:border-foreground/30 hover:text-foreground"
            title="GitHub Repository"
          >
            <GithubIcon className="h-4 w-4" />
          </a>

          {/* Mobile hamburger menu */}
          <button
            onClick={() => setMobileMenuOpen(!mobileMenuOpen)}
            className="lg:hidden inline-flex h-8 w-8 items-center justify-center rounded-md border border-border text-muted-foreground hover:text-foreground cursor-pointer"
          >
            {mobileMenuOpen ? <X className="h-4 w-4" /> : <Menu className="h-4 w-4" />}
          </button>
        </div>
      </div>

      {/* Mobile Dropdown Menu */}
      {mobileMenuOpen && (
        <div className="lg:hidden border-b border-border bg-background/95 px-4 py-3 font-mono text-xs space-y-1">
          {navLinks.map((link) => (
            <button
              key={link.id}
              onClick={() => handleNavClick(link.id)}
              className={`w-full text-left py-2 px-3 rounded-md transition-colors capitalize cursor-pointer ${
                activePage === link.id
                  ? 'bg-accent/15 text-accent font-semibold'
                  : 'text-muted-foreground hover:bg-muted/60 hover:text-foreground'
              }`}
            >
              {link.label}
            </button>
          ))}
        </div>
      )}
    </header>
  );
};
