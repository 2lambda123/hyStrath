/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016-2021 hyStrath
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of hyStrath, a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    foamListTimes

Group
    grpPostProcessingUtilities

Description
    List times using timeSelector.

Usage

    - foamListTimes [OPTION]

    \param -rm \n
    Remove selected time directories
    \param -processor \n
    List times from processor0/ directory

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "timeSelector.H"
#include "Time.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote("List times using timeSelector");

    timeSelector::addOptions(true, true);
    argList::noBanner();
    argList::noParallel();
    argList::addBoolOption
    (
        "processor",
        "list times from processor0/ directory"
    );
    argList::addBoolOption
    (
        "rm",
        "remove selected time directories"
    );
    argList::addBoolOption
    (
        "startTime",
        "select the start time"
    );
    #include "setRootCase.H"

    label nProcs = 0;

    // Create the processor databases
    PtrList<Time> databases(1);

    if (args.optionFound("processor"))
    {
        // Determine the processor count directly
        while (isDir(args.path()/(word("processor") + name(nProcs))))
        {
            ++nProcs;
        }

        if (!nProcs)
        {
            FatalErrorInFunction
                << "No processor* directories found"
                << exit(FatalError);
        }

        // Create the processor databases
        databases.setSize(nProcs);

        forAll(databases, procI)
        {
            databases.set
            (
                procI,
                new Time
                (
                    Time::controlDictName,
                    args.rootPath(),
                    args.caseName()/fileName(word("processor") + name(procI))
                )
            );
        }
    }
    else
    {
        databases.set
        (
            0,
            new Time
            (
                Time::controlDictName,
                args.rootPath(),
                args.caseName()
            )
        );
    }

    // Use the times list from the master processor
    // and select a subset based on the command-line options
    instantList timeDirs = timeSelector::select
    (
        databases[0].times(),
        args
    );

    if (args.optionFound("rm"))
    {
        if (args.optionFound("processor"))
        {
            for (label procI=0; procI<nProcs; procI++)
            {
                fileName procPath
                (
                    args.path()/(word("processor") + name(procI))
                );

                forAll(timeDirs, timeI)
                {
                    rmDir(procPath/timeDirs[timeI].name());
                }
            }
        }
        else
        {
            forAll(timeDirs, timeI)
            {
                rmDir(args.path()/timeDirs[timeI].name());
            }
        }
    }
    else
    {
        if (args.optionFound("startTime"))
        {
            Info<< timeDirs[0].name() << endl;
        }
        else
        {
            forAll(timeDirs, timeI)
            {
                Info<< timeDirs[timeI].name() << endl;
            }
        }
    }

    return 0;
}


// ************************************************************************* //
