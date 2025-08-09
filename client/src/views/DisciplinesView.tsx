import { useMemo, useState } from "preact/hooks";
import styles from "./DisciplinesView.module.scss";

import {
  Discipline,
  Centered,
  Button,
  Input,
  Spinner,
  Modal,
} from "../components";
import { Discipline as IDiscipline } from "../types";
import { icons } from "../constants";
import { caseInsensitiveIncludes } from "../utils";
import { useInputs } from "../hooks";
import DeleteDisciplineModal from "./DeleteDisciplineModal";

type ModalType = "start" | "delete";

interface DisciplinesViewProps {
  isDisciplinesLoading: boolean;
  isDisciplinesError: boolean;
  disciplines: IDiscipline[];
  onDisciplinesUpdated: (disciplines: IDiscipline[]) => void;
  onNewClick: () => void;
}

export default function DisciplinesView({
  isDisciplinesLoading,
  isDisciplinesError,
  disciplines,
  onDisciplinesUpdated,
  onNewClick,
}: DisciplinesViewProps) {
  const [inputs, onInput] = useInputs({ search: "" });
  const [modal, setModal] = useState<ModalType | null>(null);
  const [isToggleLoading, setIsToggleLoading] = useState(false);
  const [isResetLoading, setIsResetLoading] = useState(false);
  const [startingDiscipline, setStartingDiscipline] = useState<IDiscipline>();
  const [deletingDiscipline, setDeletingDiscipline] = useState<IDiscipline>();

  const filteredDisciplnies = useMemo(() => {
    return disciplines.filter(
      (discipline) =>
        caseInsensitiveIncludes(discipline.name, inputs.search) ||
        caseInsensitiveIncludes(discipline.code, inputs.search)
    );
  }, [disciplines, inputs.search]);

  function onStartClick(discipline: IDiscipline) {
    setStartingDiscipline(discipline);
    setModal("start");
  }

  async function onStartSuccess() {
    await fetch(
      `/api/start?intervals=${startingDiscipline!.intervals.join(",")}`
    );
  }

  function onStartCancel() {
    setStartingDiscipline(undefined);
    setModal(null);
  }

  function onDeleteClick(discipline: IDiscipline) {
    setDeletingDiscipline(discipline);
    setModal("delete");
  }

  function onDeleteSuccess(updatedDisciplines: IDiscipline[]) {
    onDisciplinesUpdated(updatedDisciplines);
    setModal(null);
    setDeletingDiscipline(undefined);
  }

  function onDeleteCancel() {
    setModal(null);
    setDeletingDiscipline(undefined);
  }

  async function onToggleClick() {
    try {
      setIsToggleLoading(true);
      await fetch("/api/toggle");
    } finally {
      setIsToggleLoading(false);
    }
  }

  async function onResetClick() {
    try {
      setIsResetLoading(true);
      await fetch("/api/reset");
    } finally {
      setIsResetLoading(false);
    }
  }

  // <>Are you sure you want to delete discipline{" "}<b>{deletingDiscipline!.name}</b>?</>

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
              search={inputs.search}
              discipline={discipline}
              onDeleteClick={onDeleteClick}
              onStartClick={onStartClick}
            />
          ))
        )}
      </div>
      <div className={styles.buttons}>
        <Button disabled={isToggleLoading} onClick={onToggleClick}>
          Toggle
        </Button>
        <Button disabled={isResetLoading} onClick={onResetClick}>
          Reset
        </Button>
      </div>
      {modal && (startingDiscipline || deletingDiscipline) && (
        <Modal>
          {modal === "delete" ? (
            <DeleteDisciplineModal
              deletingDiscipline={deletingDiscipline!}
              disciplines={disciplines}
              onDeleteSuccess={onDeleteSuccess}
              onDeleteCancel={onDeleteCancel}
            />
          ) : (
            <div>Start modal...</div>
          )}
        </Modal>
      )}
    </>
  );
}
