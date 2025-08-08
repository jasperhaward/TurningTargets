import { useEffect, useMemo, useState } from "preact/hooks";
import styles from "./DisciplinesView.module.scss";

import { Discipline, Centered, Button, Input, Spinner } from "../components";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";
import { caseInsensitiveIncludes } from "../utils";
import { useInputs } from "../hooks";

interface DisciplinesViewProps {
  isDisciplinesLoading: boolean;
  isDisciplinesError: boolean;
  disciplines: IDiscipline[];
  onNewClick: () => void;
}

export default function DisciplinesView({
  isDisciplinesLoading,
  isDisciplinesError,
  disciplines,
  onNewClick,
}: DisciplinesViewProps) {
  const [inputs, onInput] = useInputs({ search: "" });
  const [activeDisciplineId, setActiveDisciplineId] = useState<number>();
  const [isToggleLoading, setIsToggleLoading] = useState(false);

  const filteredDisciplnies = useMemo(() => {
    return disciplines.filter(
      (discipline) =>
        caseInsensitiveIncludes(discipline.name, inputs.search) ||
        caseInsensitiveIncludes(discipline.code, inputs.search)
    );
  }, [disciplines, inputs.search]);

  async function onDisciplineClick(discipline: IDiscipline) {
    try {
      setActiveDisciplineId(discipline.id);
      await fetch(`/api/start/${discipline.intervals.join(",")}`);
    } finally {
      setActiveDisciplineId(undefined);
    }
  }

  async function onToggleClick() {
    try {
      setIsToggleLoading(true);
      await fetch("/api/toggle");
    } finally {
      setIsToggleLoading(false);
    }
  }

  return (
    <>
      <div className={styles.actions}>
        <Input
          className={styles.search}
          name="search"
          placeholder="Search"
          icon={icons.search}
          disabled={isDisciplinesLoading || isDisciplinesError}
          value={inputs.search}
          onInput={onInput}
        />
        <Button
          className={styles.new}
          disabled={isDisciplinesLoading}
          onClick={onNewClick}
        >
          New
        </Button>
      </div>
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
              isStarting={discipline.id === activeDisciplineId}
              search={inputs.search}
              discipline={discipline}
              onClick={onDisciplineClick}
            />
          ))
        )}
      </div>
      <Button disabled={isToggleLoading} onClick={onToggleClick}>
        Toggle
      </Button>
    </>
  );
}
