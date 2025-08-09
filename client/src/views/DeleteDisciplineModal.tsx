import { useState } from "preact/hooks";
import styles from "./DeleteDisciplineModal.module.scss";

import { Button, ModalTitle } from "../components";
import { Discipline } from "../types";

interface DeleteDisciplineModalProps {
  deletingDiscipline: Discipline;
  disciplines: Discipline[];
  onDeleteSuccess: (disciplines: Discipline[]) => void;
  onDeleteCancel: () => void;
}

export default function DeleteDisciplineModal({
  deletingDiscipline,
  disciplines,
  onDeleteSuccess,
  onDeleteCancel,
}: DeleteDisciplineModalProps) {
  const [isDeleteLoading, setIsDeleteLoading] = useState(false);

  async function onDeleteClick() {
    try {
      setIsDeleteLoading(true);

      const updatedDisciplines = disciplines.filter(
        (discipline) => discipline.id !== deletingDiscipline.id
      );

      await fetch("/api/save", {
        method: "POST",
        body: JSON.stringify(updatedDisciplines),
      });

      onDeleteSuccess(updatedDisciplines);
    } finally {
      setIsDeleteLoading(false);
    }
  }

  return (
    <>
      <ModalTitle>
        Are you sure you want to delete discipline
        <b className={styles.name}>{deletingDiscipline.name}</b>?
      </ModalTitle>
      <div className={styles.buttons}>
        <Button disabled={isDeleteLoading} onClick={onDeleteClick}>
          Yes
        </Button>
        <Button onClick={onDeleteCancel}>No</Button>
      </div>
    </>
  );
}
