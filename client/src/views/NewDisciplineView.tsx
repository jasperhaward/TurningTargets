import { useState } from "preact/hooks";
import styles from "./NewDisciplineView.module.scss";

import { Discipline } from "../types";
import { Button, Input } from "../components";
import { sortAlphabetically } from "../utils";
import { icons } from "../constants";
import { useInputs } from "../hooks";

interface NewDisciplineViewProps {
  disciplines: Discipline[];
  onDisciplinesUpdated: (disciplines: Discipline[]) => void;
  onBackClick: () => void;
}

export default function NewDisciplineView({
  disciplines,
  onDisciplinesUpdated,
  onBackClick,
}: NewDisciplineViewProps) {
  const [isSaveLoading, setIsSaveLoading] = useState(false);
  const [inputs, onInput] = useInputs({ code: "", name: "", intervals: "" });

  async function onSaveClick() {
    try {
      setIsSaveLoading(true);

      const nextId = disciplines[disciplines.length - 1].id + 1;
      const newDiscipline: Discipline = {
        id: nextId,
        code: "AB12",
        name: "Example New Discipline",
        intervals: [1, 2, 3, 4],
      };

      const updatedDisciplines = sortAlphabetically([
        ...disciplines,
        newDiscipline,
      ]);

      await fetch("/api/save", {
        method: "POST",
        body: JSON.stringify(updatedDisciplines),
      });

      onDisciplinesUpdated(updatedDisciplines);
    } catch (error) {
      console.error(error);
      // todo: show toast
    } finally {
      setIsSaveLoading(false);
    }
  }

  return (
    <>
      <div className={styles.header}>
        <Button onClick={onBackClick}>{icons.chevronLeft}</Button>
        <h2>New discipline</h2>
      </div>
      <div className={styles.form}>
        <label>Code</label>
        <Input name="code" value={inputs.code} onInput={onInput} />
        <label>Name</label>
        <Input name="name" value={inputs.name} onInput={onInput} />
        <label>Intervals</label>
        <Input name="intervals" value={inputs.intervals} onInput={onInput} />
      </div>
      <Button disabled={isSaveLoading} onClick={onSaveClick}>
        Save
      </Button>
    </>
  );
}
