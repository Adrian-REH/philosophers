# Philosophers

This project is an implementation of the classic Dining Philosophers problem using semaphores for synchronization. The program simulates philosophers who alternately think and eat, requiring the use of shared resources (forks) without causing deadlock or starvation.

## Features

- **Semaphore-based synchronization**
- **Philosopher state management**
- **Error handling and debugging**

## Project Structure

```

philosophers/
├── philo/
│ ├── philo.c
│ └── ...
├── philo_bonus/
│ ├── philo_bonus.c
│ └── ...
└── README.md
```

## Getting Started

### Prerequisites

- GCC or any C compiler
- Make

### Building

To compile the project, run:

```sh
make
```

### Running

To execute the program:

```sh
./philo <number_of_philosophers>  <time_sleep>  <time_eat>  <time_die>
```

### Example

```sh
./philo 5 1000 1000 1000
```
### Test
No one should die
```sh
./philo 21 301 100 100
```
```sh
./philo 3 310 103 103
```
```sh
./philo 2 130 60 60
```
```sh
./philo 200 700 200 200
```
```sh
./philo 1 700 200 200
```
```sh
./philo 1 700 200 200
```
## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Open a pull request

## License

```

Designed and developed by 2024 adrian-reh (Adrian Ramon Elias Herrera)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

```
