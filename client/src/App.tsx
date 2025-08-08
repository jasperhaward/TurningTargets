import { useEffect, useState } from "preact/hooks";
import styles from "./App.module.scss";

import logo from "./assets/logo.png";
import { Discipline } from "./types";
import DisciplinesView from "./views/DisciplinesView";
import NewDisciplineView from "./views/NewDisciplineView";
import { sortAlphabetically } from "./utils";

export default function App() {
  const [isNewDisciplineView, setIsNewDisciplineView] = useState(false);
  const [isDisciplinesLoading, setIsDisciplinesLoading] = useState(true);
  const [isDisciplinesError, setIsDisciplinesError] = useState(false);
  const [disciplines, setDisciplines] = useState<Discipline[]>([]);

  useEffect(() => {
    loadDisciplines();
  }, []);

  async function loadDisciplines() {
    try {
      const response = await fetch("/config");

      const disciplines: Discipline[] = await response.json();

      setDisciplines(sortAlphabetically(disciplines));
    } catch (error) {
      console.error(error);
      setIsDisciplinesError(true);
    } finally {
      setIsDisciplinesLoading(false);
    }
  }

  return (
    <>
      <header className={styles.header}>
        <img alt="logo" src={logo} />
        <div>
          <h1>Turning Targets</h1>
          <p>Reigate Miniature Rifle & Pistol Club</p>
        </div>
      </header>
      {isNewDisciplineView ? (
        <NewDisciplineView
          disciplines={disciplines}
          onDisciplinesUpdated={setDisciplines}
          onNavigateToDisciplinesView={() => setIsNewDisciplineView(false)}
        />
      ) : (
        <DisciplinesView
          isDisciplinesLoading={isDisciplinesLoading}
          isDisciplinesError={isDisciplinesError}
          disciplines={disciplines}
          onNewClick={() => setIsNewDisciplineView(true)}
        />
      )}
    </>
  );
}
