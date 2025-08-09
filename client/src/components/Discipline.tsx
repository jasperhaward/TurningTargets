import styles from "./Discipline.module.scss";
import { Spinner, Button, HighlightedText } from ".";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";

interface DisciplineProps {
  search: string;
  discipline: IDiscipline;
  onDeleteClick: (discipline: IDiscipline) => void;
  onStartClick: (discipline: IDiscipline) => void;
}

export function Discipline({
  discipline,
  search,
  onDeleteClick,
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
      <div className={styles.buttons}>
        <Button onClick={() => onStartClick(discipline)}>{icons.play}</Button>
        <Button onClick={() => onDeleteClick(discipline)}>{icons.bin}</Button>
      </div>
    </div>
  );
}
