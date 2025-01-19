"use client";

import React, { useMemo } from "react";

type Step = {
  char: string;
  cell: number;
};

interface PathBoardProps {
  /**
   * Example: "p,15 l,11 o,14 n,13 k,10"
   * Means we have steps:
   *   p at cell 15,
   *   l at cell 11,
   *   o at cell 14, etc.
   */
  pathString: string;

  // Optional customizations
  rows?: number;
  cols?: number;
  cellSize?: number; // px
  cellGap?: number;  // px
}

export default function PathBoard({
  pathString,
  rows = 4,
  cols = 4,
  cellSize = 50,
  cellGap = 10,
}: PathBoardProps) {
  // Total board dimensions, factoring in gap
  const boardWidth = cols * cellSize + (cols - 1) * cellGap;
  const boardHeight = rows * cellSize + (rows - 1) * cellGap;

  // 1. Parse the path string
  const steps: Step[] = useMemo(() => {
    // e.g. "p,15 l,11 o,14..."
    return pathString.split(" ").map((part) => {
      const [char, cellStr] = part.split(",");
      return {
        char,
        cell: parseInt(cellStr, 10),
      };
    });
  }, [pathString]);

  // 2. Convert cellNumber to row/col & compute center for each step
  const stepPoints = useMemo(() => {
    return steps.map((s) => {
      const r = Math.floor(s.cell / cols);
      const c = s.cell % cols;
      // Top-left corner of cell
      const x = c * (cellSize + cellGap);
      const y = r * (cellSize + cellGap);

      return {
        ...s,
        row: r,
        col: c,
        centerX: x + cellSize / 2,
        centerY: y + cellSize / 2,
      };
    });
  }, [steps, cols, cellSize, cellGap]);

  // 3. Build arrow lines from step i to step i+1
  const lines = stepPoints.slice(0, -1).map((start, i) => {
    const end = stepPoints[i + 1];
    return {
      startX: start.centerX,
      startY: start.centerY,
      endX: end.centerX,
      endY: end.centerY,
    };
  });

  // The first step is our "start" cell
  const startRow = stepPoints[0]?.row;
  const startCol = stepPoints[0]?.col;

  return (
    <div className="relative" style={{ width: boardWidth, height: boardHeight }}>
      {/* 
        The 4x4 "grid" using CSS grid. 
        We set rows and columns to fixed cellSize, 
        and add a dynamic gap of cellGap. 
      */}
      <div
        className="absolute top-0 left-0"
        style={{
          display: "grid",
          gridTemplateRows: `repeat(${rows}, ${cellSize}px)`,
          gridTemplateColumns: `repeat(${cols}, ${cellSize}px)`,
          gap: cellGap,
          width: boardWidth,
          height: boardHeight,
        }}
      >
        {Array.from({ length: rows * cols }).map((_, idx) => {
          const r = Math.floor(idx / cols);
          const c = idx % cols;
          const stepForCell = stepPoints.find(
            (p) => p.row === r && p.col === c
          );
          // highlight start cell
          const isStart =
            r === startRow && c === startCol;

          return (
            <div
              key={idx}
              className={`rounded-lg border border-gray-300 flex items-center justify-center 
                ${isStart ? "bg-slate-500 text-white" : ""} 
              `}
            >
              {stepForCell && (
                <span className="text-xl font-bold font-mono">
                  {stepForCell.char}
                </span>
              )}
            </div>
          );
        })}
      </div>

      {/* SVG overlay for translucent white arrows */}
      <svg
        className="absolute top-0 left-0 pointer-events-none"
        width={boardWidth}
        height={boardHeight}
      >
        {lines.map((line, i) => (
          <line
            key={i}
            x1={line.startX}
            y1={line.startY}
            x2={line.endX}
            y2={line.endY}
            stroke="white"
            strokeWidth={2}
            strokeOpacity={0.6}
          />
        ))}
      </svg>
    </div>
  );
}
