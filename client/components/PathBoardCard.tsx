"use client";

import React, { useMemo } from "react";
import { Card, CardHeader, CardTitle, CardContent } from "@/components/ui/card";
import PathBoard from "@/components/PathBoard";

interface PathBoardCardProps {
  pathString: string;
}

export default function PathBoardCard({ pathString }: PathBoardCardProps) {
  // 1. Parse letters from the path string:
  //    e.g. "p,15 l,11 o,14 n,13 k,10" => letters: "plonk"
  const letters = useMemo(() => {
    return pathString
      .split(" ")
      .map((part) => part.split(",")[0]) // the char before the comma
      .join("");
  }, [pathString]);

  // 2. The "word" is just the concatenation of letters
  const word = letters; 
  // If you need uppercase/lowercase, you could do word.toUpperCase() or similar

  // 3. The number of chars
  const numChars = letters.length;

  return (
    <div className="flex justify-center p-8">
      <Card className="p-4 flex flex-row items-center gap-6">
        {/* Left Side: word name + number of chars */}
        <div className="flex flex-col min-w-[120px]">
          <CardHeader className="p-0">
            <CardTitle className="text-lg mb-2">{word}</CardTitle>
          </CardHeader>
          <p className="text-sm">
            <span className="font-semibold">Chars:</span> {numChars}
          </p>
        </div>

        {/* Right Side: The PathBoard */}
        <CardContent className="p-0">
          <PathBoard pathString={pathString} />
        </CardContent>
      </Card>
    </div>
  );
}
