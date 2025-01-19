"use client";

import React, { useState } from "react";
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
}

export default function Grid({ setBoardString, clearBoard }: GridProps) {
  const totalCells = 16;

  // State to track the input values
  const [values, setValues] = useState(Array(totalCells).fill(""));

  // Update input value and handle state
  const handleInputChange = (index: number, value: string) => {
    const newValue = value.replace(/[^a-zA-Z]/g, "").toUpperCase();
    const newValues = [...values];
    newValues[index] = newValue;
    setValues(newValues);
  };

  // Check if all inputs are filled
  const isAllFilled = values.every((val) => val !== "");

  const handleButtonClick = () => {
    const concatenated = values
      .reduce((acc, curr, idx) => {
        const rowIndex = Math.floor(idx / 4);
        acc[rowIndex] = (acc[rowIndex] || "") + curr;
        return acc;
      }, [] as string[])
      .join("|")
      .toLowerCase();
    console.log(concatenated); // Logs the concatenated string
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
