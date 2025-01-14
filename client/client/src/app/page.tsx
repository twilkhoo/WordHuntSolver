"use client";

import React, { useState, ChangeEvent } from "react";

export default function Home() {
  // Initialize a 4x4 board of empty strings
  const [board, setBoard] = useState<string[][]>(
    Array.from({ length: 4 }, () => Array(4).fill(""))
  );

  // Handle input changes in each cell
  const handleInputChange = (
    rowIndex: number,
    colIndex: number,
    event: ChangeEvent<HTMLInputElement>
  ) => {
    const { value } = event.target;
    const newBoard = board.map((row, r) =>
      row.map((cell, c) => {
        if (r === rowIndex && c === colIndex) {
          return value;
        }
        return cell;
      })
    );
    setBoard(newBoard);
  };

  // Flatten and console.log as a comma-separated string
  const handleSubmit = () => {
    const flattened = board.flat().join(",");
    console.log(flattened);
  };

  const handleClick = async () => {
    try {
      // Fetch the homepage content from localhost:3000
      const response = await fetch("http://localhost:5000/ping");
      const data = await response.text();
      console.log(data);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  }

  const handleServerAlgo = async () => {
    
    const testStr = 'a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p'
    
    try {
      // Fetch the homepage content from localhost:3000
      const response = await fetch("http://localhost:5000/" + testStr);
      const data = await response.text();
      console.log(data);
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  }

  return (
    <div style={{ padding: 20 }}>
      <h1>4×4 Board (TypeScript)</h1>
      <div style={{ display: "grid", gridTemplateRows: "repeat(4, auto)" }}>
        {board.map((row, rowIndex) => (
          <div
            key={rowIndex}
            style={{ display: "grid", gridTemplateColumns: "repeat(4, auto)" }}
          >
            {row.map((cell, colIndex) => (
              <input
                key={colIndex}
                type="text"
                value={cell}
                onChange={(e) => handleInputChange(rowIndex, colIndex, e)}
                style={{ width: 40, margin: 5 }}
              />
            ))}
          </div>
        ))}
      </div>

      <button onClick={handleSubmit} style={{ marginTop: 10 }}>
        Console Log
      </button>
      <button onClick={handleClick}>Homepage Response</button>
      <button onClick={handleServerAlgo}>Server test</button>
    </div>
  );
}
