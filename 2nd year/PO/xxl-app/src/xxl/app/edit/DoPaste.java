package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.Spreadsheet;
import xxl.exceptions.RangeException;

/**
 * Paste command.
 */
class DoPaste extends Command<Spreadsheet> {
	DoPaste(Spreadsheet receiver) {
		super(Label.PASTE, receiver);
		addStringField("range", Prompt.address());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			_receiver.paste(stringField("range"));
		} catch (RangeException e) {
			throw new InvalidCellRangeException(e.getMessage());
		}
	}
}
