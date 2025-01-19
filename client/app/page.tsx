'use client'

import React, { useState } from "react";
import Grid from "@/components/Grid";
import { Inter, Montserrat } from "next/font/google"
import PathBoard from "@/components/PathBoard";
import PathBoardCard from "@/components/PathBoardCard";

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
  const [pathsFromServer, setPathsFromServer] = useState<string[]>([]);

  const clearBoard = () => {
    setBoardString("");
    setPathsFromServer([]);
  };

  const pathStr = "p,15 l,11 o,14 n,13 k,10"

  return (
    <div>
      <div className="flex flex-col items-center mt-20">

        <div className="inline-block">
          <h1 className={montserrat.className + " text-6xl p-4"} style={{fontWeight: 700}}>Word Hunt Solver</h1>
          <h2 className={inter.className + " text-xl p-4"}>Enter your board, beat your friends.</h2>
        </div>

      </div>

      
      <Grid setBoardString={setBoardString} clearBoard={clearBoard} onFetchPaths={(paths) => setPathsFromServer(paths)}/>

      <div className="grid grid-cols-3 gap-2 mt-4">
        {pathsFromServer.map((path, index) => (
          <PathBoardCard key={index} pathString={path} />
        ))}
      </div>
    </div>
  );
}
