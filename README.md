# Simple Database Management System

## Overview
The purpose of this project is to provide a Simple Database Management System optimized for smaller data sizes and one time use of data.

This is a simple DBMS where the program controls the management and allocation of the heap memory with no explicit call to malloc(). All the databases, tables, row entries, etc. are dynamically allocated and are mapped onto the heap based on the program's custom mapper. This allows for the searching and management of different tables to be more optimized and less memory intensive. However, there is no permenant storage for information, as the heap is used to store data.

To interact with the DBMS, SQL syntax and commands are used.

## Memory Management
The program manages the layout of the heap and the size of the heap. By being able to manage the layout of the heap, blocks of memory that are related to one another (ex. table entries) can be placed next to each other to allow for easy access and traversal through these blocks. Rather than using existing data structures that use heap memory, creating custom data structures allows for creativity and strucutres that fit the design. 

## Supported Commands

| Command  | Command |
| ------------- | ------------- |
| CREATE DATABASE [Database Name]  | Creates database with specified name |
| SHOW DATABASES  | Shows current databases |
| USE DATABASE   [Database Name] | Uses the specified database  |
| CREATE TABLE [Table Name] ([Column Type List])  | Creates table in current database with specified columns |
| SHOW TABLES | Shows current tables in current database |
| INSERT INTO [Table Name] ([Column List]) Values ([Value List]) | Inserts new entry into specified table, at specified columns |
| SELECT [Column Names] FROM [Table Name] WHERE [Conditions] | Queries table and outputs specified column information |
| CHANGE PROMPT [Prompt String] | Changes prompt to user inputted prompt |
| EXIT | Exits out of the process |

## Data structures
![image](https://github.com/VenkatKunaparaju/Simple-Database-Management-System/blob/main/imgs/Legend.png?raw=true)
![image](https://github.com/VenkatKunaparaju/Simple-Database-Management-System/blob/main/imgs/Database.png?raw=true)
![image](https://github.com/VenkatKunaparaju/Simple-Database-Management-System/blob/main/imgs/Columns.png?raw=true)
![image](https://github.com/VenkatKunaparaju/Simple-Database-Management-System/blob/main/imgs/Rows.png?raw=true)
### Notes
- Valid Data Types: INT (4 bytes), DOUBLE (8 bytes), STRING (32 bytes)
- All commands need to end with ';' to execute
- Insert Statements require all columns in table to be specified currently
- Select/From statements only support single table queries currently
- Where statements only supported with AND operators currently. 



