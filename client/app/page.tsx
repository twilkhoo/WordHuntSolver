'use client'

import React, { useState } from "react";
import Grid from "@/components/Grid";
import { Inter, Montserrat } from "next/font/google"

const inter = Inter({
  subsets: ["latin"],
  weight: ["400", "700"],
});

const montserrat = Montserrat({
  subsets: ["latin"],
  weight: ["400"],
});

export default function Home() {

  const [boardString, setBoardString] = useState("");

  const clearBoard = () => {
    setBoardString("");
  };

  return (
    <div>
      <div className="flex flex-col items-center mt-20">

        <div className="inline-block">
          <h1 className={montserrat.className + " text-6xl p-4"} style={{fontWeight: 700}}>Word Hunt Solver</h1>
          <h2 className={inter.className + " text-xl p-4"}>Enter your board, beat your friends.</h2>
        </div>

      </div>

      
      <Grid setBoardString={setBoardString} clearBoard={clearBoard} />
      {/* Display the board string */}
      {boardString && (
        <p className="mt-6 text-lg font-medium">
          Board: <span className="font-mono">{boardString}</span>
        </p>
      )}
    </div>
  );
}
