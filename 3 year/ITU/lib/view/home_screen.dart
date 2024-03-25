/// Authors: 
/// Tatiana Fedorova (xfedor14@stud.fit.vutbr.cz)
/// 
import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:itu_app/view/user_input.dart';
import 'package:itu_app/model/db_helper.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      resizeToAvoidBottomInset: false,
      appBar: AppBar(
        title: const Text('CineMatch'),
        backgroundColor: const Color.fromARGB(255, 68, 70, 115),
        foregroundColor: Colors.white,
      ),
      body: Center(
        child: Container(
          padding: const EdgeInsets.all(8.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Expanded(
                child: ListView(
                  children: [
                    UserInputWidget(
                      onUserSelected: (userId) {
                        context.push('/filmGallery/$userId');
                      },
                    ),
                  ],
                ),
              ),

              ElevatedButton(
                child: const Text(
                  'View results',
                  style: TextStyle(fontSize: 25),
                ),
                onPressed: () {
                  context.push('/statsScreen');
                },
              ),
              const SizedBox(height: 20),

              ElevatedButton(
                child: const Text(
                  'List of films',
                  style: TextStyle(fontSize: 25),
                ),
                onPressed: () {
                  context.push('/listFilms');
                },
              ),
            ],
          ),
        ),
      ),
    );
  }

 
}
