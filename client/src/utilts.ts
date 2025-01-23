export function caseInsensitiveIncludes(value: string, search: string) {
  return value.toUpperCase().includes(search.toUpperCase());
}
