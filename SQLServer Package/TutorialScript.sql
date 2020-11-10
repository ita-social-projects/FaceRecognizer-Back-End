
 -- How to create a database
CREATE DATABASE MaskPhotosDatabase;

 -- How to use a database (execute this query before every next query!)
USE MaskPhotosDatabase;

-- How to create a table
CREATE TABLE Photos
(
	Id int PRIMARY KEY IDENTITY(1,1) NOT NULL, -- ID of the photo
	Photo IMAGE NOT NULL,                      -- array of bites
	PhotoName NVARCHAR(255) NOT NULL,          -- name of the photo
	PhotoExtension NVARCHAR(20) NOT NULL       -- extension of the photo
);

-- How to insert one photo into the table
INSERT INTO Photos (Photo, PhotoName, PhotoExtension) VALUES
(
(SELECT BulkColumn FROM OPENROWSET(BULK 'Path\PhotoName.PhotoExtension', SINGLE_BLOB) AS IMAGE), -- change full path
'PhotoName',                                                                                     -- change photo name
'PhotoExtension'                                                                                 -- change extension name
);

-- How to get all photos
SELECT * FROM Photos;

-- How to clear all data in the table
DELETE FROM Photos;