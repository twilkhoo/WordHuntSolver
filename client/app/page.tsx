"use client";

import React, { useState } from "react";
import Grid from "@/components/Grid";
import { Inter, Montserrat } from "next/font/google";
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

  return (
    <div>
      <div className="flex flex-col items-center mt-20">
        <div className="inline-block">
          <h1
            className={montserrat.className + " text-6xl p-4"}
            style={{ fontWeight: 700 }}
          >
            Word Hunt Solver
          </h1>
          <h2 className={inter.className + " text-xl p-4"}>
            Enter your board, beat your friends.
          </h2>
        </div>
      </div>

      <Grid
        setBoardString={setBoardString}
        clearBoard={clearBoard}
        onFetchPaths={(paths) => {
          const filteredPaths = paths.filter((path) => {
            const characters = path
              .split(" ")
              .map((p) => p.split(",")[0])
              .join("");
            return characters.length >= 3; // Keep only paths with 3+ characters.
          });
          setPathsFromServer(filteredPaths);
        }}
      />

      <div className="flex justify-center">
        <div className="grid grid-cols-3">
          {pathsFromServer.map((path, index) => (
            <PathBoardCard key={index} pathString={path} />
          ))}
        </div>
      </div>
    </div>
  );
}
