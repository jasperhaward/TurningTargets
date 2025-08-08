import { useReducer } from "preact/hooks";

export type UseInputs<T extends Record<string, string>> = [
  inputs: T,
  onInput: (event: JSX.TargetedEvent<HTMLInputElement>) => void,
  setInputs: (updatedInputs: Partial<T>) => void
];

export function useInputs<T extends Record<string, string>>(
  initialInputs: T
): UseInputs<T> {
  const [inputs, setInputs] = useReducer(
    (inputs: T, updatedInputs: Partial<T>) => ({
      ...inputs,
      ...updatedInputs,
    }),
    initialInputs
  );

  function onInput(event: JSX.TargetedEvent<HTMLInputElement>) {
    const { name, value } = event.currentTarget as HTMLInputElement;
    setInputs({ [name]: value } as Partial<T>);
  }

  return [inputs, onInput, setInputs];
}
