package xxl.parser;

import java.util.ArrayList;
import java.io.Serializable;

import xxl.cells.*;
import xxl.Spreadsheet;
import xxl.storage.CellStorage;
import xxl.exceptions.RangeException;
import xxl.exceptions.UnknownRangeException;
import xxl.exceptions.UnknownInputException;
import xxl.exceptions.InvalidResultException;
import xxl.exceptions.NonLinearRangeException;

/**
 * This class parses input and returns the respective
 * objects.
 */
public class Parser implements Serializable {
	/**
	 * Splits an entry.
	 *
	 * @param entry to split.
	 * @return [0] - position; [1] - content.
	 */
	public String[] splitEntry(String entry) {
		return entry.split("\\|");
	}

	/**
	 * Parses Content from a string.
	 *
	 * @param spreadsheet used in content initialization.
	 * @param content string representing the content.
	 * @return the content of the string.
	 *
	 * @throws RangeException when content contains an illegal interval.
	 * @throws UnknownInputException when content cannot be sucessfully parsed.
	 */
	public Content readContent(Spreadsheet spreadsheet, String content)
			throws RangeException, UnknownInputException  {
		String[] fields = content.split("=");
		if (fields.length == 1)
			return readLiteral(fields[0]);
		if (fields[0].contains("'"))
			return readLiteral(content);
		if (!fields[0].trim().equals(""))
			throw new UnknownInputException(content);
		return readFunction(spreadsheet, fields[1].trim());
	}

	/**
	 * Reads one of the sizes of an imported spreadsheet.
	 *
	 * @param entry entry to get the size from.
	 * @param position to put the read size in.
	 * @return the updated position.
	 *
	 * @throws UnknownInputException when the size doesn't match one
	 * of the following: "linhas=number" or "colunas=number".
	 */
	public void readSize(String entry, Position position) throws UnknownInputException {
		String[] fields = entry.split("=");
		if (fields.length == 1 && !entry.trim().equals(""))
			throw new UnknownInputException(entry);
		switch (fields[0].trim()) {
		case "linhas" -> {
			try {
				position.setRow(Integer.parseInt(fields[1].trim()));
			} catch (NumberFormatException e) {
				throw new UnknownInputException(entry);
			}
		}
		case "colunas" -> {
			try {
				position.setColumn(Integer.parseInt(fields[1].trim()));
			} catch (NumberFormatException e) {
				throw new UnknownInputException(entry);
			}
		}
		default -> throw new UnknownInputException(entry);
		}
	}

	/**
	 * Reads a Position from a string in the form of "row;column".
	 *
	 * @param position string representing the position.
	 * @return the corresponding position.
	 *
	 * @throws RangeException when the position cannot be read.
	 */
	private Position readPosition(String position) throws RangeException {
		String[] fields = position.trim().split(";");
		if (fields.length == 1)
			throw new UnknownRangeException(position);

		try {
			int row = Integer.parseInt(fields[0]) - 1;
			int column = Integer.parseInt(fields[1]) - 1;
			return new Position(row, column);
		} catch (NumberFormatException e) {
			throw new UnknownRangeException(position);
		}
	}

	/**
	 * Reads Intervals in the form of "row1;column1:row2;column2"
	 * (can also read "row;column" which is equivalent to
	 * "row;column:row;column").
	 * 
	 * @param interval string containing the interval.
	 * @return corresponding Interval.
	 *
	 * @throws RangeException when the interval is not linear or unparsable.
	 */
	public Interval readInterval(String stringInterval) throws RangeException {
		Interval interval;
		try {
			if (!stringInterval.contains(":")) {
				Position pos = readPosition(stringInterval);
				interval = new Interval(pos, pos);
			} else {
				String[] fields = stringInterval.split(":");
				if (fields.length != 2)
					throw new UnknownRangeException(stringInterval);
				Position pos1 = readPosition(fields[0]);
				Position pos2 = readPosition(fields[1]);
				interval = new Interval(pos1, pos2);
			}
			if (!interval.isLinear()) {
				throw new NonLinearRangeException(stringInterval);
			}
			return interval;
		} catch (UnknownRangeException e) {
			throw new UnknownRangeException(stringInterval);
		}
	}

	/**
	 * Reads a function from a string.
	 *
	 * @param spreadsheet to be used in Content initialization.
	 * @param function string to parse the Content from.
	 * @return the Content containing the respective function.
	 *
	 * @throws RangeException when the function uses an illegal interval.
	 * @throws UnknownInputException  when there is no adequate function.
	 */
	private Content readFunction(Spreadsheet spreadsheet, String function)
				throws RangeException, UnknownInputException {
		String[] fields = function.split("\\(");
		if (fields[0].equals(function))
			return new ReferenceContent(readPosition(fields[0]),
							spreadsheet.getStorage());
		fields[1] = fields[1].replace(")", "");
		if (isInterval(fields[1]))
			return readRange(spreadsheet, fields[0].trim(), fields[1]);
		return readBinary(spreadsheet, fields[0].trim(), fields[1]);
	}

	/**
	 * Reads a literal from a string.
	 *
	 * @param literal string to parse the literal from.
	 * @return Content containing the respective literal.
	 *
	 * @param UnknownInputException when no adequate literal exists.
	 */
	public Content readLiteral(String literal) throws UnknownInputException {
		if (literal.contains("'")) {
			String[] fields = literal.split("'", 2);
			if (!fields[0].trim().equals(""))
				throw new UnknownInputException(literal);
			return new StringContent(fields[1]);
		}
		
		try {
			return new IntegerContent(Integer.parseInt(literal.trim()));
		} catch (NumberFormatException e) {
			throw new UnknownInputException(literal);
		}
	}

	/**
	 * Checks if a String has the format "number;number:number;number".
	 *
	 * @param interval string to check.
	 * @return true if it does; false otherwise.
	 */
	private boolean isInterval(String interval) {
		return interval.trim().matches("[0-9]+;[0-9]+:[0-9]+;[0-9]+");
	}

	/**
	 * Reads Binary function arguments (literals or references).
	 *
	 * @param spreadsheet used in Content initialization.
	 * @param args arguments to parse.
	 * @return list containing the parsed Content.
	 *
	 * @throws RangeException when a reference points out of bounds.
	 * @throws UnknownInputException when an argument cannot be parsed.
	 */
	private ArrayList<Content> readArguments(Spreadsheet spreadsheet, String... args)
					throws RangeException, UnknownInputException {
		ArrayList<Content> parsedArgs = new ArrayList<>();
		for (String arg : args) {
			try {
				parsedArgs.add(readLiteral(arg));
			} catch (UnknownInputException e) {
				if (arg.contains(";"))
					parsedArgs.add(new ReferenceContent(readPosition(arg),
								spreadsheet.getStorage()));
				else
					throw e;
			}
		}
		return parsedArgs;
	}

	/**
	 * Reads a Binary function.
	 *
	 * @param spreadsheet used in Content initialization
	 * @param name of the function
	 * @param arguments of the function
	 * @return content containing the function
	 *
	 * @throws RangeException when a reference argument points to an out of bounds position.
	 * @throws UnknownInputException when no existing function matches the name of the given function
	 */
	private Content readBinary(Spreadsheet spreadsheet, String name, String arguments)
				throws RangeException, UnknownInputException {
		var args = readArguments(spreadsheet, arguments.split(","));
		if (args.size() != 2)
			throw new UnknownInputException(name);
		return switch (name) {
		case "ADD" -> new IntegerBinary(args.get(0), args.get(1),
				new BinaryOperation<Integer>("ADD") {
				@Override
				public Integer execute(Integer operand1, Integer operand2)
								throws InvalidResultException {
					return operand1 + operand2;
				}
			});

		case "SUB" -> new IntegerBinary(args.get(0), args.get(1),
				new BinaryOperation<Integer>("SUB") {
				@Override
				public Integer execute(Integer operand1, Integer operand2)
								throws InvalidResultException {
					return operand1 - operand2;
				}
			});

		case "MUL" -> new IntegerBinary(args.get(0), args.get(1),
				new BinaryOperation<Integer>("MUL") {
				@Override
				public Integer execute(Integer operand1, Integer operand2)
								throws InvalidResultException {
					return operand1 * operand2;
				}
			});

		case "DIV" -> new IntegerBinary(args.get(0), args.get(1),
				new BinaryOperation<Integer>("DIV") {
				@Override
				public Integer execute(Integer operand1, Integer operand2)
								throws InvalidResultException {
					if (operand2 == 0)
						throw new InvalidResultException();
					return operand1 / operand2;
				}
			});

		default -> throw new UnknownInputException(name);
		};
	}

	/**
	 * Reads a function that function that executes over intervals.
	 *
	 * @param spreadsheet used for content initialization.
	 * @param name of the function.
	 * @param interval string representing the interval.
	 * @return content containing the function.
	 *
	 * @throws RangeException when the used interval is illegal.
	 * @throws UnknownInputException when no existing function 
	 * matches the name of the function.
	 */
	private Content readRange(Spreadsheet spreadsheet, String name, String interval)
					throws RangeException, UnknownInputException {
		Interval range = readInterval(interval);
		CellStorage storage = spreadsheet.getStorage();
		return switch(name) {
		case "AVERAGE" -> new AverageRange(range, storage);
		case "PRODUCT" -> new ProductRange(range, storage);
		case "CONCAT" -> new ConcatRange(range, storage);
		case "COALESCE" -> new CoalesceRange(range, storage);
		default -> throw new UnknownInputException(name);
		};
	}
}
