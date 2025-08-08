import styles from "./Input.module.scss";

export interface InputProps {
  className?: string;
  name: string;
  placeholder?: string;
  icon?: JSX.Element;
  disabled?: boolean;
  value: string;
  onInput: (event: JSX.TargetedEvent<HTMLInputElement>) => void;
}

export function Input({
  className = "",
  name,
  placeholder,
  icon,
  disabled,
  value,
  onInput,
}: InputProps) {
  return (
    <div class={`${styles.input} ${className}`}>
      {icon}
      <input
        name={name}
        placeholder={placeholder}
        disabled={disabled}
        value={value}
        autoComplete="off"
        onInput={onInput}
      />
    </div>
  );
}
