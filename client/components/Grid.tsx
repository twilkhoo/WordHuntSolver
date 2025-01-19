"use client";

import React, { useRef, useState } from "react";
import { Input } from "@/components/ui/input";
import { Button } from "@/components/ui/button";
import { Tooltip, TooltipContent, TooltipProvider, TooltipTrigger } from "@/components/ui/tooltip";
import { Roboto } from "next/font/google";

const roboto = Roboto({
  subsets: ["latin"],
  weight: ["400", "700"],
});

interface GridProps {
  setBoardString: (boardString: string) => void;
  clearBoard: () => void;
  onFetchPaths: (paths: string[]) => void;
}

export default function Grid({ setBoardString, clearBoard, onFetchPaths }: GridProps) {
  const totalCells = 16;

  // State to track the input values
  const [values, setValues] = useState(Array(totalCells).fill(""));

  const inputRefs = useRef<HTMLInputElement[]>([]);

  // Update input value and handle state
  const handleInputChange = (index: number, value: string) => {
    const newValue = value.replace(/[^a-zA-Z]/g, "").toUpperCase();
    const newValues = [...values];
    newValues[index] = newValue;
    setValues(newValues);

    if (value.length === 1 && index < totalCells - 1) {
      inputRefs.current[index + 1]?.focus();
    }
  };

  // Check if all inputs are filled
  const isAllFilled = values.every((val) => val !== "");

  const handleButtonClick = async () => {
    const concatenated = values
      .reduce((acc, curr, idx) => {
        const rowIndex = Math.floor(idx / 4);
        acc[rowIndex] = (acc[rowIndex] || "") + curr;
        return acc;
      }, [] as string[])
      .join(",")
      .toLowerCase();
    console.log(concatenated); // Logs the concatenated string

    
    try {
      const res = await fetch(`http://localhost:5000/${concatenated}`);
      const data = await res.text();

      // data is a giant string separated by "|"
      // 3) Split
      const pathStrings = data.split("|").filter((s) => s.trim().length > 0);

      // 4) Pass the resulting array of path strings to the parent
      onFetchPaths(pathStrings);
    } catch (err) {
      console.error("Error fetching from server:", err);
    }

    setBoardString(concatenated);
  };

  const handleClear = () => {
    setValues(Array(totalCells).fill(""));
    clearBoard(); // Clear the board string in the parent component
  };

  return (
    <div className="flex flex-col items-center justify-center p-4">
      {/* 4x4 Grid */}
      <div className="grid grid-cols-4 gap-4 mb-4">
        {Array.from({ length: totalCells }).map((_, index) => (
          <Input
            key={index}
            ref={(el) => {inputRefs.current[index] = el!}}
            type="text"
            maxLength={1}
            className={`${roboto.className} !text-2xl aspect-square text-center w-20 h-20`}
            value={values[index]}
            onChange={(e) => handleInputChange(index, e.target.value)}
          />
        ))}
      </div>

      {/* Tooltip and Button */}
      <TooltipProvider>
        <Tooltip>
          <TooltipTrigger asChild>
            <div className="inline-block">
              <Button
                onClick={handleButtonClick}
                disabled={!isAllFilled}
                className="px-6 py-2 text-lg font-semibold"
              >
                Submit
              </Button>
            </div>
          </TooltipTrigger>
          {!isAllFilled && (
            <TooltipContent side="top">
              Must enter all chars before submitting
            </TooltipContent>
          )}
        </Tooltip>
      </TooltipProvider>

      <Button
          onClick={handleClear}
          variant="outline"
          className="px-6 py-2 text-lg font-semibold"
        >
          Clear
      </Button>
    </div>
  );
}
