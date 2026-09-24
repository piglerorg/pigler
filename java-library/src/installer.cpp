#define ODC_NAME "javapiglerapi.odc"

#include <e32base.h>
#include <f32file.h>
#include <bautils.h>
#include <aknglobalnote.h>

void InstallL()
{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	_LIT(KJrtDir,  "C:\\resource\\java\\jvm\\lib\\jrt\\");
	TBool copied = BaflUtils::FolderExists(fs, KJrtDir) && BaflUtils::FileExists(fs, _L("C:\\resource\\java\\jvm\\lib\\javafile.odc"));
	if (!copied) {
		fs.MkDirAll(KJrtDir);
		CFileMan* fm = CFileMan::NewL(fs);
		fm->Copy(_L("Z:\\resource\\java\\jvm\\lib\\jrt\\*"), KJrtDir, CFileMan::ERecurse | CFileMan::EOverWrite);
		delete fm;
	}

	_LIT(KList, "C:\\resource\\java\\midpodclist");
	RFile list;
	TInt err = list.Open(fs, KList, EFileRead | EFileWrite);
	if (err == KErrNotFound) {
		err = list.Create(fs, KList, EFileWrite);
		if (!err) {
			list.Write(_L8(ODC_NAME "\r\n"));
			list.Close();
		}
	} else if (!err) {
		CleanupClosePushL(list);

		TInt size = 0;
		list.Size(size);
		if (size != 0) {
			HBufC8* buffer = HBufC8::NewLC(size);
			TPtr8 ptr = buffer->Des();
			list.Read(ptr);
			ptr.LowerCase();
			if (ptr.Find(_L8(ODC_NAME)) == KErrNotFound) {
				TInt pos = 0;
				list.Seek(ESeekEnd, pos);
				list.Write(_L8(ODC_NAME "\r\n"));
			}
			CleanupStack::PopAndDestroy(buffer);
		} else {
			list.Write(_L8(ODC_NAME "\r\n"));
		}

		CleanupStack::PopAndDestroy(&list);
	}

	CAknGlobalNote* note = CAknGlobalNote::NewLC();
	if (err) {
		note->ShowNoteL(EAknGlobalErrorNote, _L("Open4All not enabled"));
	} else {
		note->ShowNoteL(EAknGlobalInformationNote, _L("Installation done"));
	}
	CleanupStack::PopAndDestroy(note);
	CleanupStack::PopAndDestroy(&fs);
}

TInt E32Main()
{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (!cleanup) return KErrNoMemory;

	CActiveScheduler* scheduler = new CActiveScheduler();
	if (!scheduler) {
		delete cleanup;
		return KErrNoMemory;
	}
	CActiveScheduler::Install(scheduler);
	TRAPD(err, InstallL());

	delete scheduler;
	delete cleanup;

	return err;
}
