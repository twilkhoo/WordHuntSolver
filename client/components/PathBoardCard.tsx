"use client";

import React, { useMemo } from "react";
import { Card, CardHeader, CardTitle, CardContent } from "@/components/ui/card";
import PathBoard from "@/components/PathBoard";
import { Inter, Montserrat } from "next/font/google"

const inter = Inter({
  subsets: ["latin"],
  weight: ["400", "700"],
});


const montserrat = Montserrat({
  subsets: ["latin"],
  weight: ["400"],
});

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


  const charsToPoints = new Map();
  
  charsToPoints.set(3, 100);
  charsToPoints.set(4, 400);
  charsToPoints.set(5, 800);
  charsToPoints.set(6, 1400);
  charsToPoints.set(7, 1800);
  charsToPoints.set(8, 2200);
  charsToPoints.set(9, 2200);
  charsToPoints.set(10, 2200);
  charsToPoints.set(11, 2200);
  charsToPoints.set(12, 2200);
  charsToPoints.set(13, 2200);
  charsToPoints.set(14, 2200);
  charsToPoints.set(15, 2200);
  charsToPoints.set(16, 2200);

  return (
    <div className="flex justify-center m-4">
      <Card className="p-4 flex flex-row items-center gap-6">
        {/* Left Side: word name + number of chars */}
        <div className="flex flex-col min-w-[120px]">
          <CardHeader className="p-0">
            <CardTitle className={montserrat.className + " text-2xl mb-2"}>{word}</CardTitle>
          </CardHeader>
          <p className="text-sm">
            <span className="font-semibold">Chars:</span> {numChars}
          </p>
          <p className="text-sm">
            <span className="font-semibold">Points:</span> {charsToPoints.get(numChars)}
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
