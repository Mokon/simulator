#!/usr/bin/perl
# Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved

use strict;
use warnings;

use Switch;
use TestCase;

my @tests;

# All the test cases are stored in a csv file so its easier to modify them. Each
# test case is stored in four consective lines and must be parsed as such.

open( my $data, '<', "tests.csv" ) or die "Could not open tests\n";

my (
    $num_simulations,  $general_roll,     $risk_europe,
    $risk,             $verbose,          $stats,
    $attacker_peasent, $attacker_archer,  $attacker_infantry,
    $attacker_cavalry, $attacker_pikemen, $attacker_siege,
    $defender_peasent, $defender_archer,  $defender_infantry,
    $defender_cavalry, $defender_pikemen, $defender_siege,
    $draws,            $attacker_wins,    $defense_wins,
    $avg_rounds
);

my $record_line = 0;

while ( my $line = <$data> ) {
    chomp $line;

    next if ( $line =~ m/^#/ );
    next if ( $line eq "" );

    ++$record_line;
    switch ($record_line) {
        case 1 {
            (
                $num_simulations, $general_roll, $risk_europe, $risk, $verbose,
                $stats
            ) = split ",", $line;
        }
        case 2 {
            (
                $attacker_peasent, $attacker_archer,  $attacker_infantry,
                $attacker_cavalry, $attacker_pikemen, $attacker_siege
            ) = split ",", $line;
        }
        case 3 {
            (
                $defender_peasent, $defender_archer,  $defender_infantry,
                $defender_cavalry, $defender_pikemen, $defender_siege
            ) = split ",", $line;
        }
        case 4 {
            ( $draws, $attacker_wins, $defense_wins, $avg_rounds ) = split ",",
              $line;

            push(
                @tests,
                new TestCase(
                    $num_simulations,  $general_roll,     $risk_europe,
                    $risk,             $verbose,          $stats,
                    $attacker_peasent, $attacker_archer,  $attacker_infantry,
                    $attacker_cavalry, $attacker_pikemen, $attacker_siege,
                    $defender_peasent, $defender_archer,  $defender_infantry,
                    $defender_cavalry, $defender_pikemen, $defender_siege,
                    $draws,            $attacker_wins,    $defense_wins,
                    $avg_rounds
                )
            );

            $record_line = 0;
        }
    }
}

foreach my $test (@tests) {
    $test->run();
}
