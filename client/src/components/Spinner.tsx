import styles from "./Spinner.module.scss";

export interface SpinnerProps {
  small?: boolean;
}

export function Spinner({ small }: SpinnerProps) {
  return <div class={`${styles.spinner} ${small ? styles.small : ""}`} />;
}
