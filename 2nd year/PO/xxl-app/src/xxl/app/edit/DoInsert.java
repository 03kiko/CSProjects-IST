package xxl.app.edit;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.Spreadsheet;
import xxl.exceptions.RangeException;
import xxl.exceptions.UnknownInputException;

/**
 * Class for inserting data.
 */
class DoInsert extends Command<Spreadsheet> {

    DoInsert(Spreadsheet receiver) {
        super(Label.INSERT, receiver);
        addStringField("range", Prompt.address());
        addStringField("content", Prompt.content());
    }

    @Override
    protected final void execute() throws CommandException {
        try {
            _receiver.insertContents(stringField("range"),
                                     stringField("content"));
	} catch (UnknownInputException badContent) {
		throw new UnknownFunctionException(badContent.getMessage());
        } catch (RangeException badRange) {
		throw new InvalidCellRangeException(badRange.getMessage());
	}
    }
}
