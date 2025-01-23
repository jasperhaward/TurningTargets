import styles from "./Input.module.scss";

export interface InputProps {
  placeholder: string;
  icon: JSX.Element;
  disabled: boolean;
  value: string;
  setValue: (value: string) => void;
}

export function Input({
  placeholder,
  icon,
  disabled,
  value,
  setValue,
}: InputProps) {
  function onInput(event: JSX.TargetedEvent<HTMLInputElement>) {
    setValue(event.currentTarget.value);
  }

  return (
    <div class={styles.input}>
      {icon}
      <input
        placeholder={placeholder}
        disabled={disabled}
        value={value}
        onInput={onInput}
      />
    </div>
  );
}
