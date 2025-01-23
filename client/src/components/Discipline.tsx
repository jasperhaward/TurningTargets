import styles from "./Discipline.module.scss";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";
import { HighlightedText } from "./HighlightedText";
import { Spinner } from "./Spinner";

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
        <button onClick={() => onClick(discipline)}>{icons.play}</button>
      )}
    </div>
  );
}
