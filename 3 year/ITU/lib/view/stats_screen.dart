/// Authors: 
/// Aleksandr Shevchenko (xshevc01@stud.fit.vutbr.cz)
/// 
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:itu_app/controller/film_controller.dart';
import 'package:itu_app/controller/user_controller.dart';
import 'package:itu_app/model/db_helper.dart';
import 'package:itu_app/model/film_model.dart';
import 'package:provider/provider.dart';
import 'package:sqflite/sqflite.dart';

import 'package:itu_app/common/film_card_widgets.dart';

class StatsScreen extends StatelessWidget {
  const StatsScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Results'),
        backgroundColor: const Color.fromARGB(255, 68, 70, 115),
        foregroundColor: Colors.white,
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            const SizedBox(height: 20),
            const Text(
              'The best match is:',
              style: TextStyle(fontSize: 25, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 10),
            Expanded(
              child: BestMatchCard(),
            ),
          ],
        ),
      ),
    );
  }
}

class BestMatchCard extends StatelessWidget {
  double _percentageLikes = 0.0;

  BestMatchCard({super.key});

  @override
  Widget build(BuildContext context) {
    return FutureBuilder<Film?>(
      future: getBestMatchFilm(context),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const CircularProgressIndicator();
        } else if (snapshot.hasError ||
            !snapshot.hasData ||
            snapshot.data == null) {
          return const Text('No user films available.');
        }

        final bestMatchFilm = snapshot.data!;

        return Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          children: [
            const SizedBox(height: 5),
            Text(
              'Percentage of Likes: ${_percentageLikes.toStringAsFixed(2)}%',
              style: const TextStyle(fontSize: 20),
            ),
            const SizedBox(height: 10),
            InkWell(
              onTap: () {
                int filmId = bestMatchFilm.id!;
                context.push('/listFilms/$filmId/comments');
              },
              child: SizedBox(
                height: 520,
                width: 300,
                child: SingleChildScrollView(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      FilmPoster(posterPath: bestMatchFilm.posterPath),
                      const SizedBox(height: 10),
                      Center(
                        child: Text(
                          bestMatchFilm.title,
                          style: const TextStyle(
                            fontSize: 30,
                            fontWeight: FontWeight.bold,
                          ),
                          textAlign: TextAlign.center,
                        ),
                      ),
                      const SizedBox(height: 5),
                      FilmAttribute(
                          attribName: "Overview: ",
                          attribValue: bestMatchFilm.overview),
                      const SizedBox(height: 5),
                      FilmAttribute(
                          attribName: "Release Date: ",
                          attribValue: bestMatchFilm.releaseDate),
                      const SizedBox(height: 5),
                      FilmAttribute(
                          attribName: "Actors: ",
                          attribValue: bestMatchFilm.actors),
                      const SizedBox(height: 5),
                      FilmAttribute(
                          attribName: "Director: ",
                          attribValue: bestMatchFilm.director),
                      const SizedBox(height: 5),
                      FilmAttribute(
                          attribName: "Duration: ",
                          attribValue: bestMatchFilm.duration),
                      const SizedBox(height: 10),
                      CommentsWidget(filmId: bestMatchFilm.id!),
                    ],
                  ),
                ),
              ),
            ),
          ],
        );
      },
    );
  }

  Future<Film?> getBestMatchFilm(BuildContext context) async {
    final db = await context.read<DatabaseHelper>().database;
    final films = await context.read<FilmController>().films();
    final users =
        await context.read<UserController>().getUsers();

    final percentageLikes = <int, double>{};

    for (final film in films) {
      final totalLikes = Sqflite.firstIntValue(await db.rawQuery('''
        SELECT COUNT(*) FROM user_films
        WHERE film_id = ? AND choice = 1
      ''', [film.id])) ?? 0;

      final totalDislikes = Sqflite.firstIntValue(await db.rawQuery('''
        SELECT COUNT(*) FROM user_films
        WHERE film_id = ? AND choice = 0
      ''', [film.id])) ?? 0;

      final totalUsers = users.length;

      final totalVotes = totalLikes + totalDislikes;
      final percentage = totalVotes > 0 ? totalLikes / totalVotes * 100.0 : 0.0;

      percentageLikes[film.id!] = percentage;
    }

    final bestMatchFilmId =
        percentageLikes.entries.reduce((a, b) => a.value > b.value ? a : b).key;

    final bestMatchFilm = films.firstWhere(
      (film) => film.id == bestMatchFilmId,
    );

    _percentageLikes = percentageLikes[bestMatchFilmId] ?? 0.0;

    return bestMatchFilm;
  }
}
