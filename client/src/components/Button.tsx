import { ComponentChildren } from "preact";
import styles from "./Button.module.scss";

export interface ButtonProps {
  className?: string;
  disabled?: boolean;
  children: ComponentChildren;
  onClick: () => void;
}

export function Button({
  className = "",
  disabled,
  children,
  onClick,
}: ButtonProps) {
  return (
    <button
      className={`${styles.button} ${className}`}
      disabled={disabled}
      onClick={onClick}
    >
      {children}
    </button>
  );
}
