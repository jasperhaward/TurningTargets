import styles from "./Discipline.module.scss";
import { Spinner, Button, HighlightedText } from ".";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";

interface DisciplineProps {
  isStarting: boolean;
  search: string;
  discipline: IDiscipline;
  onStartClick: (discipline: IDiscipline) => void;
}

export function Discipline({
  isStarting,
  discipline,
  search,
  onStartClick,
}: DisciplineProps) {
  return (
    <div className={styles.discipline}>
      <div className={styles.detailsContainer}>
        <div className={styles.details}>
          <HighlightedText className={styles.name} query={search}>
            {discipline.name}
          </HighlightedText>
          <HighlightedText className={styles.code} query={search}>
            {discipline.code}
          </HighlightedText>
        </div>
        <div className={styles.intervals}>
          {discipline.intervals.join(", ")}
        </div>
      </div>
      {isStarting ? (
        <Spinner small />
      ) : (
        // TODO: add delete button with confirmation modal
        <Button onClick={() => onStartClick(discipline)}>{icons.play}</Button>
      )}
    </div>
  );
}
