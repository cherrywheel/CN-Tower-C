import { useState, useEffect } from 'react';
import { Header } from './components/Header';
import { Footer } from './components/Footer';
import { HomePage } from './pages/HomePage';
import { DownloadPage } from './pages/DownloadPage';
import { TargetsPage } from './pages/TargetsPage';
import { CanaryPage } from './pages/CanaryPage';
import { BuildPage } from './pages/BuildPage';
import { PlayPage } from './pages/PlayPage';
import { PythonPage } from './pages/PythonPage';
import { LegalPage } from './pages/LegalPage';
import { getInitialLanguage, saveLanguage, type Language, type PageId } from './i18n';

function parseHash(): PageId {
  if (typeof window === 'undefined') return 'home';
  const raw = window.location.hash.replace(/^#\/?/, '').toLowerCase().trim();
  const validPages: PageId[] = ['home', 'download', 'targets', 'canary', 'build', 'play', 'python', 'legal'];
  if (validPages.includes(raw as PageId)) {
    return raw as PageId;
  }
  return 'home';
}

export function App() {
  const [lang, setLang] = useState<Language>(getInitialLanguage);
  const [theme, setTheme] = useState<'dark' | 'light' | 'system'>('system');
  const [page, setPage] = useState<PageId>(parseHash);

  // Sync hash routing
  useEffect(() => {
    const onHashChange = () => {
      setPage(parseHash());
      window.scrollTo({ top: 0, behavior: 'smooth' });
    };

    window.addEventListener('hashchange', onHashChange);
    return () => window.removeEventListener('hashchange', onHashChange);
  }, []);

  const navigateTo = (targetPage: PageId) => {
    setPage(targetPage);
    window.location.hash = `#/${targetPage}`;
    window.scrollTo({ top: 0, behavior: 'smooth' });
  };

  const handleSelectLang = (selectedLang: Language) => {
    setLang(selectedLang);
    saveLanguage(selectedLang);
  };

  // Handle theme application
  useEffect(() => {
    const root = document.documentElement;

    const applyTheme = () => {
      if (theme === 'dark') {
        root.classList.add('dark');
        root.classList.remove('light');
      } else if (theme === 'light') {
        root.classList.add('light');
        root.classList.remove('dark');
      } else {
        const isSystemLight = window.matchMedia && window.matchMedia('(prefers-color-scheme: light)').matches;
        if (isSystemLight) {
          root.classList.add('light');
          root.classList.remove('dark');
        } else {
          root.classList.add('dark');
          root.classList.remove('light');
        }
      }
    };

    applyTheme();

    if (theme === 'system') {
      const mediaQuery = window.matchMedia('(prefers-color-scheme: light)');
      const listener = () => applyTheme();
      mediaQuery.addEventListener('change', listener);
      return () => mediaQuery.removeEventListener('change', listener);
    }
  }, [theme]);

  return (
    <div className="min-h-screen bg-background text-foreground transition-colors duration-200 flex flex-col justify-between">
      <div>
        <Header
          lang={lang}
          onSelectLang={handleSelectLang}
          theme={theme}
          onChangeTheme={setTheme}
          activePage={page}
          onNavigate={navigateTo}
        />
        <main>
          {page === 'home' && <HomePage lang={lang} onNavigate={navigateTo} />}
          {page === 'download' && <DownloadPage lang={lang} onNavigate={navigateTo} />}
          {page === 'targets' && <TargetsPage lang={lang} onNavigate={navigateTo} />}
          {page === 'canary' && <CanaryPage lang={lang} onNavigate={navigateTo} />}
          {page === 'build' && <BuildPage lang={lang} onNavigate={navigateTo} />}
          {page === 'play' && <PlayPage lang={lang} onNavigate={navigateTo} />}
          {page === 'python' && <PythonPage lang={lang} onNavigate={navigateTo} />}
          {page === 'legal' && <LegalPage lang={lang} onNavigate={navigateTo} />}
        </main>
      </div>
      <Footer lang={lang} onNavigate={navigateTo} />
    </div>
  );
}

export default App;
