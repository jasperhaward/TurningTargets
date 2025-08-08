import styles from "./Discipline.module.scss";
import { Spinner, Button, HighlightedText } from ".";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";

interface DisciplineProps {
  isStarting: boolean;
  search: string;
  discipline: IDiscipline;
  onClick: (discipline: IDiscipline) => void;
}

export function Discipline({
  isStarting,
  discipline,
  search,
  onClick,
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
        <Button onClick={() => onClick(discipline)}>{icons.play}</Button>
      )}
    </div>
  );
}
