import { useState } from "preact/hooks";
import styles from "./NewDisciplineView.module.scss";

import { Discipline } from "../types";
import { Button, Input } from "../components";
import { sortAlphabetically } from "../utils";
import { icons } from "../constants";
import { useInputs } from "../hooks";

/**
 * Returns true for: 1 , 123,123
 * Returns false for: 0 , 123,
 */
const INTERVALS_REGEX = /^[1-9][0-9]*(,[1-9][0-9]*)*$/;

interface NewDisciplineViewProps {
  disciplines: Discipline[];
  onDisciplinesUpdated: (disciplines: Discipline[]) => void;
  onNavigateToDisciplinesView: () => void;
}

export default function NewDisciplineView({
  disciplines,
  onDisciplinesUpdated,
  onNavigateToDisciplinesView,
}: NewDisciplineViewProps) {
  const [isSaveLoading, setIsSaveLoading] = useState(false);
  const [inputs, onInput] = useInputs({ code: "", name: "", intervals: "" });

  const isIntervalsCommaSeparatedIntegers = INTERVALS_REGEX.test(
    inputs.intervals
  );
  // even number of commas = odd number of intervals
  const numberOfCommas = (inputs.intervals.match(/,/g) || []).length;
  const isIntervalsEvenNumberOfCommas = numberOfCommas % 2 === 0;

  const parsedIntervals = isIntervalsCommaSeparatedIntegers
    ? inputs.intervals.split(",").map(parseFloat)
    : [];

  const isIntervalsLessThan300 = !parsedIntervals.find(
    (interval) => interval > 300
  );

  const isFormValid =
    inputs.code !== "" &&
    inputs.name !== "" &&
    isIntervalsCommaSeparatedIntegers &&
    isIntervalsEvenNumberOfCommas &&
    isIntervalsLessThan300;

  async function onSaveClick() {
    try {
      setIsSaveLoading(true);

      const disciplineIds = disciplines.map((discipline) => discipline.id);
      const nextId = Math.max(...disciplineIds) + 1;

      const newDiscipline: Discipline = {
        id: nextId,
        code: inputs.code,
        name: inputs.name,
        intervals: parsedIntervals,
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
      onNavigateToDisciplinesView();
    } finally {
      setIsSaveLoading(false);
    }
  }

  return (
    <>
      <div className={styles.header}>
        <Button onClick={onNavigateToDisciplinesView}>
          {icons.chevronLeft}
        </Button>
        <h2>New discipline</h2>
      </div>
      <div className={styles.form}>
        <label>Code</label>
        <Input
          placeholder="1902"
          name="code"
          value={inputs.code}
          onInput={onInput}
        />
        <label>Name</label>
        <Input
          placeholder="Advancing Target"
          name="name"
          value={inputs.name}
          onInput={onInput}
        />
        <label>Intervals</label>
        <Input
          placeholder="10,5,8,5,6"
          name="intervals"
          value={inputs.intervals}
          onInput={onInput}
        />
        <p className={styles.intervalsDescription}>
          Must contain only positive numbers and commas, and be made up of an
          odd number of intervals. Each number is the length of an interval in
          seconds and must not be greater than 300 seconds.
        </p>
      </div>
      <Button disabled={!isFormValid || isSaveLoading} onClick={onSaveClick}>
        Save
      </Button>
    </>
  );
}
