import { ComponentChildren } from "preact";
import styles from "./Modal.module.scss";

export interface ModalProps {
  children: ComponentChildren;
}

export function Modal({ children }: ModalProps) {
  return (
    <div class={styles.curtain}>
      <div class={styles.modal}>{children}</div>
    </div>
  );
}

export interface ModalTitleProps {
  children: ComponentChildren;
}

export function ModalTitle({ children }: ModalTitleProps) {
  return <div class={styles.title}>{children}</div>;
}
