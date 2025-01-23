import { useEffect, useMemo, useState } from "preact/hooks";
import styles from "./App.module.scss";

import logo from "./assets/logo.png";
import { Discipline, Input, Spinner } from "./components";
import { Discipline as IDiscipline } from "./types";
import { icons } from "./constants";
import { caseInsensitiveIncludes } from "./utilts";
import { Centered } from "./components/Centered";

export default function App() {
  const [search, setSearch] = useState("");
  const [isDisciplinesLoading, setIsDisciplinesLoading] = useState(true);
  const [isDisciplinesError, setIsDisciplinesError] = useState(false);
  const [disciplines, setDisciplines] = useState<IDiscipline[]>([]);
  const [isToggleLoading, setIsToggleLoading] = useState(false);
  const [startingDisciplineId, setStartingDisciplineId] = useState<number>();

  useEffect(() => {
    loadDisciplines();
  }, []);

  const filteredDisciplnies = useMemo(() => {
    return disciplines.filter(
      (discipline) =>
        caseInsensitiveIncludes(discipline.name, search) ||
        caseInsensitiveIncludes(discipline.code, search)
    );
  }, [disciplines, search]);

  async function loadDisciplines() {
    try {
      const response = await fetch("/config");
      const disciplines: IDiscipline[] = await response.json();
      setDisciplines(disciplines);
    } catch (error) {
      console.error(error);
      setIsDisciplinesError(true);
    } finally {
      setIsDisciplinesLoading(false);
    }
  }

  async function onDisciplineClick(discipline: IDiscipline) {
    try {
      setStartingDisciplineId(discipline.id);
      await fetch(`/start/${discipline.intervals.join(",")}`);
    } finally {
      setStartingDisciplineId(undefined);
    }
  }

  async function onToggleClick() {
    try {
      setIsToggleLoading(true);
      await fetch("/toggle");
    } finally {
      setIsToggleLoading(false);
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
      <Input
        placeholder="Search"
        icon={icons.search}
        disabled={isDisciplinesLoading || isDisciplinesError}
        value={search}
        setValue={setSearch}
      />
      <div className={styles.disciplines}>
        {isDisciplinesLoading ? (
          <Centered>
            <Spinner />
          </Centered>
        ) : isDisciplinesError ? (
          <Centered>
            <p>Failed to load disciplines.</p>
          </Centered>
        ) : filteredDisciplnies.length === 0 ? (
          <Centered>
            <p>No disciplines found.</p>
          </Centered>
        ) : (
          filteredDisciplnies.map((discipline) => (
            <Discipline
              key={discipline.id}
              isStarting={discipline.id === startingDisciplineId}
              search={search}
              discipline={discipline}
              onClick={onDisciplineClick}
            />
          ))
        )}
      </div>
      <button
        className={styles.toggle}
        disabled={isToggleLoading}
        onClick={onToggleClick}
      >
        Toggle
      </button>
    </>
  );
}
