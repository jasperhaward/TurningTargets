export interface HighlightedTextProps {
  className?: string;
  query: string;
  children: string;
}

export function HighlightedText({
  className,
  query,
  children,
}: HighlightedTextProps) {
  if (children === "") {
    return <div className={className}>{children}</div>;
  }

  // regex capture group to split children by query but
  // also include the query value in the returned array
  const regex = new RegExp(`(${query})`, "i");

  return (
    <div className={className}>
      {children.split(regex).map((substring, index) => {
        if (substring.toUpperCase() === query.toUpperCase()) {
          return <mark key={index}>{substring}</mark>;
        }

        return substring;
      })}
    </div>
  );
}
