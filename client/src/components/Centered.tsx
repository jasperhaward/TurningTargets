import { ComponentChildren } from "preact";
import styles from "./Centered.module.scss";

export interface CenteredProps {
  children: ComponentChildren;
}

export function Centered({ children }: CenteredProps) {
  return <div class={styles.centered}>{children}</div>;
}
