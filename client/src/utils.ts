import { Discipline } from "./types";

export function caseInsensitiveIncludes(value: string, search: string) {
  return value.toUpperCase().includes(search.toUpperCase());
}

export function sortAlphabetically(disciplines: Discipline[]) {
  return disciplines.sort((a, b) => a.name.localeCompare(b.name));
}
