import { useEffect, useState } from "preact/hooks";
import styles from "./StartDisciplineModal.module.scss";

import { Button, ModalTitle } from "../components";
import { Discipline } from "../types";

interface StartDisciplineModalProps {
  startingDiscipline: Discipline;
  onStartSuccess: () => void;
  onStartCancel: () => void;
}

export default function StartDisciplineModal({
  startingDiscipline,
  onStartSuccess,
  onStartCancel,
}: StartDisciplineModalProps) {
  const [countdown, setCountdown] = useState(10);

  useEffect(() => {
    const interval = setInterval(() => {
      setCountdown((countdown) => countdown - 1);
    }, 1000);

    return () => clearTimeout(interval);
  }, []);

  useEffect(() => {
    if (countdown === 0) {
      startDiscipline();
    }
  }, [countdown]);

  async function startDiscipline() {
    const intervals = startingDiscipline.intervals.join(",");

    await fetch(`/api/start?intervals=${intervals}`);

    onStartSuccess();
  }

  return (
    <>
      <ModalTitle>
        Starting <b>{startingDiscipline.name}</b>...
      </ModalTitle>
      <div className={styles.countdown}>
        <span>{countdown}</span>
      </div>
      <Button className={styles.cancel} onClick={onStartCancel}>
        Cancel
      </Button>
    </>
  );
}
